#version 330 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _uv;
layout(location = 2) in vec3 _normal;

out vec2 uv;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _projection;

uniform mat3 _normalModel;

void main(){
	gl_Position = _projection * _view * _model * vec4(_position, 1.0);
	fragPosition = vec3(_model * vec4(_position, 1.0));
	uv = _uv;
	normal = _normalModel * _normal;
}
