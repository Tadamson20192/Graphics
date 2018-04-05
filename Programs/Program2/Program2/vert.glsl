#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;


out vec3 ourColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 scale;
uniform mat4 view;
uniform mat4 projection;

void main() {
	
	gl_Position = projection * view * scale * vec4(aPos, 1.0);
	FragPos = vec3(scale * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(scale))) * aNormal; 
    ourColor = vec3(0, 1, 0);
}