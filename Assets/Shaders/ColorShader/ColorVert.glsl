#version 330 core

layout(location = 0) in vec3 _position;

uniform mat4 _transform;

void main(){
    gl_Position = _transform * vec4(_position, 1.0);
}
