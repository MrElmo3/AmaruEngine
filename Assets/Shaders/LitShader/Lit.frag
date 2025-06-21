#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform vec3 _lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 _color = vec3(1.0, 1.0, 1.0);
uniform sampler2D _texture;

void main() {
	FragColor = texture(_texture, uv) * vec4(_color * _lightColor, 1.0);
}