#version 420 core

layout(location = 0) in vec3 _position;

layout (std140, binding = 0) uniform CameraBlock {
	mat4 _view;
	mat4 _projection;
	vec3 _viewPosition;
};

uniform mat4 _model;

void main(){
    gl_Position = _projection * _view * _model * vec4(_position, 1.0);
}
