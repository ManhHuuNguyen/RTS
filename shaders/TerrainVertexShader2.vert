#version 430 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture;

uniform mat4 viewMat;
uniform mat4 projection;

uniform float density;
uniform float gradient;

varying vec4 positionEyeCoord;
varying vec4 normalEyeCoord;
varying	vec2 texCoord;
varying float visibility;

void main() {
	mat4 modelView = viewMat;
	positionEyeCoord = modelView * vec4(position.x, 0.0, position.y, 1.0);
	float distanceFromCamera = length(positionEyeCoord.xyz);
	visibility = clamp(exp(-pow(distanceFromCamera*density, gradient)), 0.0, 1.0);
	gl_Position = projection * positionEyeCoord;
	normalEyeCoord = modelView * vec4(0.0, 1.0, 0.0, 0.0); // 0.0, 1.0, 0.0 is normal
	texCoord = texture;
}