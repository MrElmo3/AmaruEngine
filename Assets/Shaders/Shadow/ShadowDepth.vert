#version 420 core

layout(location = 0) in vec3 _position;

uniform mat4 _lightSpaceMatrix;
uniform mat4 _model;

void main(){
	gl_Position = _lightSpaceMatrix * _model * vec4(_position, 1.0);
}
