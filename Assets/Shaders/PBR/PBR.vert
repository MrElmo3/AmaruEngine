#version 420 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _uv;

out vec2 uv;
out vec3 normalModel;
out vec3 fragPosition;

layout (std140, binding = 0) uniform CameraBlock {
	mat4 _view;
	mat4 _projection;
	vec3 _viewPosition;
};

uniform mat4 _model;
uniform mat3 _normalModel;

void main(){
	gl_Position = _projection * _view * _model * vec4(_position, 1.0);
	fragPosition = vec3(_model * vec4(_position, 1.0));
	uv = _uv;
	normalModel = _normalModel * _normal;
}
