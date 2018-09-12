#version 430 core
layout(location = 0) in vec3 position;

uniform mat4 viewMat;
uniform mat4 projection;

void main() {
	gl_Position = projection * viewMat * vec4(position, 1.0);
}