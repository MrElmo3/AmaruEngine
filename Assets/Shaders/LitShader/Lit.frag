#version 420 core
out vec4 FragColor;

struct Material {
	vec3 color;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

//enumglBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
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
in vec3 normal;
in vec3 fragPosition;

layout (std140, binding = 0) uniform CameraBlock {
	mat4 _view;
	mat4 _projection;
	vec3 _viewPosition;
};

layout (std140, binding = 1) uniform LightBlock {
	Light _activeLights[MAX_LIGHTS];
};

uniform Material _material;


vec3 CalcLight(Light light, vec3 normal, vec3 viewDirection, vec3 fragPosition);

void main() {
	vec3 norm = normalize(normal);
	vec3 viewDirection = normalize(_viewPosition - fragPosition);

	vec3 result;

	for (int i = 0; i < MAX_LIGHTS; i++) {
		Light currentLight = _activeLights[i];

		if(currentLight.type == DISABLE){
			continue;
		}
		result += CalcLight(currentLight, norm, viewDirection, fragPosition);
	}
	
	//emission
	vec3 emission = texture(_material.emission, uv).rgb;

	FragColor = vec4(result, 1.0);
}

vec3 CalcLight(Light light, vec3 normal, vec3 viewDirection, vec3 fragPosition) {
	
	vec3 lightDirection;

	if(light.type == DIRECTIONAL) {
		lightDirection = normalize(-light.direction);
	}
	else{
		lightDirection = normalize(light.position - fragPosition);
	}

	//diffuse
	float diff = max(dot(normal, lightDirection), 0.0);

	//specular
	vec3 reflectDirection = reflect(-1 * lightDirection, normal);
	float spec = pow(
		max(dot(viewDirection, reflectDirection), 0.0), 
		_material.shininess);
	
	vec3 ambient =  light.ambient * texture(_material.diffuse, uv).rgb;
	vec3 diffuse = light.diffuse * diff * texture(_material.diffuse, uv).rgb;
	vec3 specular = light.specular * spec * texture(_material.diffuse, uv).rgb;

	if(light.type != DIRECTIONAL) {
		// atenuation
		float distance    = length(light.position - fragPosition);
		float atenuation = 1.0 / (light.constantAtenuation + 
			light.linearAtenuation * distance + 
  			light.quadraticAtenuation * (distance * distance)
		);

		diffuse *= atenuation;
		specular *= atenuation;

		if(light.type == SPOT) {
			//Spot soft edges
			float theta = dot(lightDirection, normalize(-light.direction)); 
			float epsilon = (light.innerRange - light.outerRange);
			float intensity = clamp((theta - light.outerRange) / epsilon, 0.0, 1.0);
			diffuse  *= intensity;
			specular *= intensity;
		}
		return (diffuse + specular);
	}
	return (ambient + diffuse + specular);
}