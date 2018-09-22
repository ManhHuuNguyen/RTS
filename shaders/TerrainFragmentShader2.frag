#version 430 core

uniform vec4 lightSourcePosition;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform sampler2D texture0;

uniform uint texturesOn[4];
uniform vec4 fogColor;

varying vec4 positionEyeCoord;
varying vec4 normalEyeCoord;
varying	vec2 texCoord;
varying float visibility;

void main(){
	vec4 l = normalize(lightSourcePosition - positionEyeCoord);
	vec4 normalFrag = normalize(normalEyeCoord);
	vec2 texCoordClamp = texCoord;
	texCoordClamp.x = clamp(texCoord.x, 0.00005, 0.99995);
	texCoordClamp.y = clamp(texCoord.y, 0.00005, 0.99995);
	
	vec2 tileCoords = texCoord*40;
	vec4 textureKD = vec4(texture(texture0, tileCoords).rgba);
	
	
	vec4 diffuse = lightIntensity * textureKD * max(0.0, dot(l, normalFrag));
	vec4 ambient = ambientIntensity * textureKD;
	gl_FragColor = ambient + diffuse;
	gl_FragColor = mix(fogColor, gl_FragColor, visibility);
}