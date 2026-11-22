#version 420 core
out vec4 FragColor;

struct Material {
	vec3 color;
	sampler2D albedoMap;
	sampler2D normalMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D aoMap;
	float shininess;
};

struct FragInfo {
	vec3 position;

	vec3 albedo;
	vec3 normal;
	float metallic;
	float roughness;
	float ao;

	vec3 reflectance;
};

//enum
const uint DISABLE = 0u;
const uint DIRECTIONAL = 1u;
const uint POINT = 2u;
const uint SPOT = 3u;

//number of active lights differents to directional
const int MAX_LIGHTS = 8;

struct Light {
	vec3 position;
	uint type;

	vec3 direction;
	float innerRange;

	vec3 color;
	float outerRange;

	vec3 ambient;
	float constantAtenuation;

	vec3 diffuse;
	float linearAtenuation;

	vec3 specular;
	float quadraticAtenuation;
};

in vec2 uv;
in vec3 normalModel;
in vec3 fragPosition;

layout (std140, binding = 0) uniform CameraBlock {
	mat4 _view;
	mat4 _projection;
	vec3 _viewPosition;
};

uniform Material _material;

layout (std140, binding = 1) uniform LightBlock {
	Light _activeLights[MAX_LIGHTS];
};

// Shadow of the directional light (set by the renderer, off by default)
uniform bool _receiveShadows;
uniform mat4 _lightSpaceMatrix;
uniform sampler2D _shadowMap;

const float PI = 3.14159265359;

vec3 GetNormalFromMap();
float CalcShadow(vec3 lightDirection);

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 CalcLight(
	FragInfo fragInfo,
	Light light, 
	vec3 viewDirection
);

void main() {
	FragInfo fragInfo;
	vec3 albedoAux = texture(_material.albedoMap, uv).rgb;
	fragInfo.albedo = pow(albedoAux, vec3(2.2));
    fragInfo.metallic = texture(_material.metallicMap, uv).r;
    fragInfo.roughness = texture(_material.roughnessMap, uv).r;
    fragInfo.ao = texture(_material.aoMap, uv).r;

    fragInfo.normal = GetNormalFromMap();
    vec3 viewDirection = normalize(_viewPosition - fragPosition);

    fragInfo.reflectance = vec3(0.04); 
    fragInfo.reflectance = mix(
		fragInfo.reflectance, 
		fragInfo.albedo,
		fragInfo.metallic
	);

    // reflectance equation
	vec3 Lo = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
		Light currentLight = _activeLights[i];

		if(currentLight.type == DISABLE){
			continue;
		}
		Lo += CalcLight(fragInfo, _activeLights[i], viewDirection);
	}

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.003) * fragInfo.albedo * fragInfo.ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}

vec3 GetNormalFromMap() {
	vec3 tangentNormal =  texture(_material.normalMap, uv).xyz;
	tangentNormal = tangentNormal  * 2.0 - 1.0;

	vec3 Q1  = dFdx(fragPosition);
	vec3 Q2  = dFdy(fragPosition);
	vec2 st1 = dFdx(uv);
	vec2 st2 = dFdy(uv);

	vec3 N   = normalize(normalModel);
	vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
	vec3 B  = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

// Returns how much the fragment is in shadow: 0 = fully lit, 1 = fully shadowed.
float CalcShadow(vec3 lightDirection) {
	if (!_receiveShadows) return 0.0;

	vec4 fragPositionLight = _lightSpaceMatrix * vec4(fragPosition, 1.0);
	vec3 projCoords = fragPositionLight.xyz / fragPositionLight.w;
	projCoords = projCoords * 0.5 + 0.5;

	// Outside of the far plane of the light: nothing can occlude it
	if (projCoords.z > 1.0) return 0.0;

	// Bigger bias on surfaces almost parallel to the light to avoid shadow acne
	float NdotL = max(dot(normalize(normalModel), lightDirection), 0.0);
	float bias = max(0.004 * (1.0 - NdotL), 0.001);

	// 3x3 PCF to soften the edges
	vec2 texelSize = 1.0 / vec2(textureSize(_shadowMap, 0));
	float shadow = 0.0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float closestDepth = texture(_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (projCoords.z - bias > closestDepth) ? 1.0 : 0.0;
		}
	}
	return shadow / 9.0;
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CalcLight(
	FragInfo fragInfo,
	Light light, 
	vec3 viewDirection 
) {
	
	vec3 lightDirection;

	if(light.type == DIRECTIONAL) {
		lightDirection = normalize(-light.direction);
	}
	else{
		lightDirection = normalize(light.position - fragPosition);
	}

	// calculate per-light radiance
	vec3 halfwayDirection = normalize(viewDirection + lightDirection);

	vec3 radiance = light.color;

	if(light.type == DIRECTIONAL){
		radiance *= 1.0 - CalcShadow(lightDirection);
	}

	if(light.type != DIRECTIONAL){
		float distance = length(light.position - fragPosition);
		float atenuation = 1.0 / (light.constantAtenuation + 
			light.linearAtenuation * distance + 
  			light.quadraticAtenuation * (distance * distance)
		);

		radiance = radiance * atenuation;

		if(light.type == SPOT) {
			//Spot soft edges
			float theta = dot(lightDirection, normalize(-light.direction)); 
			float epsilon = (light.innerRange - light.outerRange);
			float intensity = clamp((theta - light.outerRange) / epsilon, 0.0, 1.0);
			radiance *= intensity;
		}
	}

	// -  Cook-Torrance BRDF
	//Normal distribution function (Microfacet model)
	float NDF = DistributionGGX(
		fragInfo.normal, 
		halfwayDirection, 
		fragInfo.roughness);
	
	//Geometry function (microfacet model)
	float G = GeometrySmith(
		fragInfo.normal, 
		viewDirection, 
		lightDirection, 
		fragInfo.roughness
	);
	
	vec3 F = fresnelSchlick(
		max(dot(halfwayDirection, viewDirection), 0.0), fragInfo.reflectance
	);

	vec3 numerator = NDF * G * F; 
	// + 0.0001 to prevent divide by zero
	float denominator = 
		4.0 * 
		max(dot(fragInfo.normal, viewDirection), 0.0) * 
		max(dot(fragInfo.normal, lightDirection), 0.0) + 0.0001; 
	vec3 specular = numerator / denominator;
	
	// kS is equal to Fresnel
	vec3 kS = F;
	
	vec3 kD = vec3(1.0) - kS;
	
	kD *= 1.0 - fragInfo.metallic;	  

	// scale light by NdotL
	float NdotL = max(dot(fragInfo.normal, lightDirection), 0.0);        

	// add to outgoing radiance Lo
	vec3 lightOutput = (kD * fragInfo.albedo / PI + specular) * radiance * NdotL;
	return lightOutput;
}