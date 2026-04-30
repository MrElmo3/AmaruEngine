#version 420 core

uniform vec3 _color = vec3(1.0, 1.0, 1.0);

out vec4 FragColor;

void main(){
    FragColor = vec4(_color, 1.0);
}