#version 430 core

uniform vec4 lightSourcePosition;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D blendMap;

uniform uint texturesOn[4];
uniform uint numTexture;
uniform vec4 fogColor;

varying vec4 positionEyeCoord;
varying vec4 normalEyeCoord;
varying	vec2 texCoord;
varying float visibility;

void main(){
	vec4 l = normalize(lightSourcePosition - positionEyeCoord);
	vec4 normalFrag = normalize(normalEyeCoord);
	vec4 textureKD = vec4(1.0, 0.0, 0.0, 0.0);
	vec2 texCoordClamp = texCoord;
	texCoordClamp.x = clamp(texCoord.x, 0.00005, 0.99995);
	texCoordClamp.y = clamp(texCoord.y, 0.00005, 0.99995);
	if (numTexture == 1){
		vec2 tileCoords = texCoord*40;
		textureKD = vec4(texture(texture0, tileCoords).rgba);
	}
	else{
		vec4 blendMapColor = vec4(texture(blendMap, texCoord).rgba);
		float blendMapColorw = 1.0 - (texturesOn[1] * blendMapColor.x + texturesOn[2] * blendMapColor.y + texturesOn[3] * blendMapColor.z);
		vec2 tileCoords = texCoord*40;
		vec4 c0 = texturesOn[0] * blendMapColorw * vec4(texture(texture0, tileCoords).rgba);
		vec4 c1 = texturesOn[1] * blendMapColor.x * vec4(texture(texture1, tileCoords).rgba);
		vec4 c2 = texturesOn[2] * blendMapColor.y * vec4(texture(texture2, tileCoords).rgba);
		vec4 c3 = texturesOn[3] * blendMapColor.z * vec4(texture(texture3, tileCoords).rgba);
		textureKD = c0 + c1 + c2 + c3;
	}
	vec4 diffuse = lightIntensity * textureKD * max(0.0, dot(l, normalFrag));
	vec4 ambient = ambientIntensity * textureKD;
	gl_FragColor = ambient + diffuse;
	gl_FragColor = mix(fogColor, gl_FragColor, visibility);
	
}