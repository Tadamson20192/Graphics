#version 330 core





in vec3 Normal;
in vec3 FragPos;


vec3 lightPos = vec3(-0.5f, 0.5f, 1.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
vec3 viewPos = vec3(0.0f, 0.0f, 0.5f);
vec3 ourColor = vec3(0.0f,1.0f,0.0f);

out vec4 fragColor;

void main() {
	
	float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

	    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
	
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  	
	
    vec3 result = (ambient + diffuse+ specular) * ourColor;
    fragColor = vec4(result, 1.0f);
}