#version 330 core

in vec2 TextCoords;
out vec4 FragColor;

uniform sampler2D _texture;
uniform vec3 _color;

void main(){
    vec4 sampled = texture(_texture, TextCoords);
    FragColor = sampled * vec4(_color, 1.0);
}