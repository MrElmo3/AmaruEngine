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
const uint numberActiveLights = 4u;

struct Light {
	uint type;

	vec3 position;
	vec3 direction;
	float cutOff;

	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec2 uv;
in vec3 normal;
in vec3 fragPosition;

uniform vec3 _viewPosition = vec3(0.0, 0.0, 0.0);

uniform Material _material;

uniform Light _directionalLight;
uniform Light _activeLights[numberActiveLights];

vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDirection);
vec3 CalcActiveLights(Light light, vec3 normal, vec3 fragPosition, vec3 viewDirection);

void main() {
	vec3 norm = normalize(normal);
	vec3 viewDirection = normalize(_viewPosition - fragPosition);

	vec3 result = CalcDirectionalLight(_directionalLight, norm, viewDirection);

	//emission
	vec3 emission = texture(_material.emission, uv).rgb;

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDirection){
	if(light.type != 0u) return vec3(0);
	
	vec3 lightDirection = normalize(-light.direction);

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
	return (ambient + diffuse + specular);
}

