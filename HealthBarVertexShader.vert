#version 430 core
layout(location = 0) in vec2 position;

uniform mat4 transformMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projection;


void main() {
	gl_Position = projection * viewMat * modelMat * transformMat * vec4(position, 0.0, 1.0);
}