#version 430 core

varying	vec2 texCoord;
uniform sampler2D guiTexture;
uniform vec4 color;
uniform uint hasTexture;


void main(){
	gl_FragColor = texture(guiTexture,texCoord);
}