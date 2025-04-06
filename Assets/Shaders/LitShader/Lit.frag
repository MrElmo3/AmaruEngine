#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragPosition;
out vec4 FragColor;

uniform float _ambientStrength = 1.0f;

uniform vec3 _lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 _lightPosition = vec3(0.0, 0.0, 0.0);
uniform float _lightRange = 1.0f;
uniform float _lightIntensity = 1.0f;

uniform vec3 _color = vec3(1.0, 1.0, 1.0);
uniform sampler2D _texture;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(_lightPosition - fragPosition);
    float lightDistance = distance(_lightPosition, fragPosition);

    vec3 ambient = _ambientStrength * _lightColor;
    float diff = 0.0;
    if(lightDistance <= _lightRange) {
        diff = max(dot(norm, lightDirection), diff);
    }
    vec3 diffuse = diff * _lightColor * _lightIntensity;
    vec3 result = (ambient + diffuse) * _color;

    FragColor = texture(_texture, uv) * vec4(result, 1.0);
}