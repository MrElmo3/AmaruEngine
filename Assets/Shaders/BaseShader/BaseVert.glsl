#version 330 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _uv;
out vec2 TextCoords;

uniform mat4 _transform;

void main(){
    gl_Position = _transform * vec4(_position, 1.0);
    TextCoords = _uv;
}
