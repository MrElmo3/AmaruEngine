#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragPosition;
out vec4 FragColor;

uniform vec3 _viewPosition = vec3(0.0, 0.0, 0.0);

uniform float _ambientStrength = 1.0;
uniform float _specularStrength = 0.5;
uniform uint _shininess = 32u;

uniform vec3 _viewPosition = vec3(0.0, 0.0, 0.0);

uniform float _ambientStrength = 1.0;
uniform float _specularStrength = 0.5;
uniform uint _shininess = 32u;

uniform vec3 _lightPosition = vec3(0.0, 0.0, 0.0);
uniform vec3 _lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 _lightPosition = vec3(0.0, 0.0, 0.0);
uniform float _lightRange = 1.0;
uniform float _lightIntensity = 1.0;

uniform vec3 _color = vec3(1.0, 1.0, 1.0);
uniform sampler2D _texture;

void main() {
	vec3 norm = normalize(normal);

	vec3 lightDirection = normalize(_lightPosition - fragPosition);
	float lightDistance = distance(_lightPosition, fragPosition);

	vec3 viewDirection = normalize(_viewPosition - fragPosition);
	vec3 reflectDirection = reflect(-1 * lightDirection, norm);

	vec3 ambient = _ambientStrength * _lightColor;
	float diff = 0.0;
	float spec = 0.0;
	if(lightDistance <= _lightRange) {
		diff = max(dot(norm, lightDirection), diff);
		spec = pow(max(dot(viewDirection, reflectDirection), 0.0), _shininess);
	}
	vec3 diffuse = diff * _lightColor * _lightIntensity;
	vec3 specular = spec * _lightColor * _specularStrength;
	vec3 result = (ambient + diffuse + specular) * _color;

	FragColor = texture(_texture, uv) * vec4(result, 1.0);
}