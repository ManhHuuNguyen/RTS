#version 430 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture;
layout(location = 3) in ivec4 jointIDs;
layout(location = 4) in vec4 weights;

uniform mat4 viewMat;
uniform mat4 projection;

uniform mat4 modelMat;
uniform mat4 joints[30];
uniform uint animated;

uniform float density;
uniform float gradient;

varying vec4 positionEyeCoord;
varying vec4 normalEyeCoord;
varying	vec2 texCoord;
varying float visibility;

void main() {
	mat4 jointMatrix = mat4(1.0);
	if (animated == 1){
		jointMatrix = animated * (joints[jointIDs.w] * weights.w + joints[jointIDs.z] * weights.z + joints[jointIDs.y] * weights.y + joints[jointIDs.x] * weights.x);
	}
	mat4 modelView = viewMat * modelMat;
	positionEyeCoord = modelView * jointMatrix * position;
	float distanceFromCamera = length(positionEyeCoord.xyz);
	visibility = clamp(exp(-pow(distanceFromCamera*density, gradient)), 0.0, 1.0);
	gl_Position = projection * positionEyeCoord;
	normalEyeCoord = modelView  * jointMatrix * normal;
	texCoord = texture;
}