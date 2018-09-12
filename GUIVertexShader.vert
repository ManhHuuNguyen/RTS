#version 430 core
layout(location = 0) in vec2 position;

uniform mat4 transformMat;

varying	vec2 texCoord;



void main() {
	texCoord = vec2((position.x+1.0)/2.0, 1 - (position.y+1.0)/2.0);
	gl_Position = transformMat * vec4(position, 0.0, 1.0);
	
}