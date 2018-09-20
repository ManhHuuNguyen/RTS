#version 430 core

uniform vec4 lightSourcePosition;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec3 KD;
uniform vec3 KA;
uniform vec4 fogColor;

uniform uint hasTexture;

uniform sampler2D textureSampler;

varying vec4 positionEyeCoord;
varying vec4 normalEyeCoord;
varying	vec2 texCoord;
varying float visibility;

void main(){
	vec4 l = normalize(lightSourcePosition - positionEyeCoord);
	vec4 normalFrag = normalize(normalEyeCoord);
	if (hasTexture == 1){
		vec4 textureKD = vec4(texture(textureSampler, texCoord).rgba);
		vec4 diffuse = lightIntensity * textureKD * max(0.0, dot(l, normalFrag));
		vec4 ambient = ambientIntensity * textureKD;
		gl_FragColor = ambient + diffuse;
	}
	else {
		vec4 diffuse = lightIntensity * vec4(KD, 0.0) * max(0.0, dot(l, normalFrag));
		vec4 ambient = ambientIntensity * vec4(KA, 0.0);
		gl_FragColor = ambient + diffuse;
	}
	gl_FragColor = mix(fogColor, gl_FragColor, visibility);
}