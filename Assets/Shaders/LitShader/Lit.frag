#version 330 core

struct Material {
	vec3 color;
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 uv;
in vec3 normal;
in vec3 fragPosition;
out vec4 FragColor;

uniform vec3 _viewPosition = vec3(0.0, 0.0, 0.0);

uniform Material _material;
uniform Light _light;  

void main() {
	// ambient
	vec3 ambient =  _light.ambient * vec3(texture(_material.diffuse, uv));

	//diffuse
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(_light.position - fragPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = _light.diffuse * (diff * vec3(texture(_material.diffuse, uv)));

	// spec
	vec3 viewDirection = normalize(_viewPosition - fragPosition);
	vec3 reflectDirection = reflect(-1 * lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), _material.shininess);
	vec3 specular = spec * _light.specular * _material.specular;

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}