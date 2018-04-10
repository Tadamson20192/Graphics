#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atexture;



out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 scale;

void main() {
	FragPos = vec3(scale * vec4(aPos, 1.0));
	gl_Position = scale * vec4(aPos, 1.0);
	TexCoord = atexture;
}