#version 330 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _uv;

out vec2 uv;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _projection;

void main(){
	gl_Position = _projection * _view * _model * vec4(_position, 1.0);
	uv = _uv;
}
