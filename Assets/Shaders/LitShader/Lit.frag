#version 330 core
out vec4 FragColor;

struct Material {
	vec3 color;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

//enum
const uint Directional = 0u;
const uint Point = 1u;
const uint Spot = 2u;

//number of active lights differents to directional
const int numberActiveLights = 4;

struct Light {
	uint type;

	vec3 position;
	vec3 direction;

	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float linear;
	float quadratic;

	float innerRange;
	float outerRange;
};

in vec2 uv;
in vec3 normal;
in vec3 fragPosition;

uniform vec3 _viewPosition = vec3(0.0, 0.0, 0.0);

uniform Material _material;

uniform Light _directionalLight;
uniform Light _activeLights[numberActiveLights];

vec3 CalcLight(Light light, vec3 normal, vec3 viewDirection, vec3 fragPosition);

void main() {
	vec3 norm = normalize(normal);
	vec3 viewDirection = normalize(_viewPosition - fragPosition);

	//directional light
	vec3 result = CalcLight(_directionalLight, norm, viewDirection, fragPosition);

	// other lights
	for(int i = 0; i < numberActiveLights; i++)
        result += CalcLight(_activeLights[i], norm, viewDirection, fragPosition);

	//emission
	vec3 emission = texture(_material.emission, uv).rgb;

	FragColor = vec4(result, 1.0);
}

vec3 CalcLight(Light light, vec3 normal, vec3 viewDirection, vec3 fragPosition) {
	
	vec3 lightDirection;

	if(light.type == Directional) {
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

	if(light.type != Directional) {
		// atenuation
		float distance    = length(light.position - fragPosition);
		float atenuation = 1.0 / (1.0 + 
			light.linear * distance + 
  			light.quadratic * (distance * distance)
		);

		diffuse *= atenuation;
		specular *= atenuation;

		if(light.type == Spot) {
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