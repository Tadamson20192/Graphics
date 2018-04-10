#version 330 core

out vec4 fragColor;


in vec2 TexCoord;


uniform sampler2D ourTexture;

void main() {
	
	//fragColor = vec4(ourTexture.x,ourTexture.y, 0, 1);
	fragColor = texture(ourTexture, TexCoord);
}