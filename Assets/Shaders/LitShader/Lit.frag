#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragPosition;
out vec4 FragColor;

uniform float _ambientStrength = 1.0f;
uniform vec3 _lightPosition = vec3(0.0, 0.0, 0.0);
uniform vec3 _lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 _color = vec3(1.0, 1.0, 1.0);
uniform sampler2D _texture;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(_lightPosition - fragPosition);

    vec3 ambient = _ambientStrength * _lightColor;

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * _lightColor;

    vec3 result = (ambient + diffuse) * _color;

    FragColor = texture(_texture, uv) * vec4(result, 1.0);
}