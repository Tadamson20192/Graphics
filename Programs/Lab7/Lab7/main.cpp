

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <readFile.h>
#include <shader.h>



int screenWidth = 640;
int screenHeight = 480;

int shadeState = 2;

float translateX = 0.0f;
float translateY = 0.0f;

GLuint checkers[320 * 240];

std::vector<float> cubeVec;
std::vector<float> dinoVec;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	screenWidth = height;
	screenHeight = width;
}

void processInput(GLFWwindow *window, Shader &shader) {

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (translateY < 20) {
			translateY = translateY + .1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (translateY > -20) {
			translateY = translateY - .1;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (translateX < 20) {
			translateX = translateX + .1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (translateX > -20) {
			translateX = translateX - .1;

		}
	}



}

float convertX(float x) {
	float temp = x / screenWidth;
	temp = -1 + (temp * 2);
	return temp;
}
float convertY(float y) {
	float temp = 1 - (2 * (y / screenHeight));
	return temp;
}

void fillCheckers() {
	int xCounter = 0, yCounter = 0;
	bool flag = true;
	GLuint color = 0xFFFFFFFF;
	
	for (int i = 0; i < 240; ++i) {
		++yCounter;
		if (yCounter >= 24) {
			if (flag) {
				flag = false;
				color = 0x00000000;
			}
			else {
				flag = true;
				color = 0xFFFFFFFF;
			}
		}
		
		for (int j = 0; j < 320; ++j) {
			checkers[i * 320 + j] = color;
			++xCounter;
			if (xCounter >= 32) {
				if (flag) {
					flag = false;
					color = 0x00000000;
				}
				else {
					flag = true;
					color = 0xFFFFFFFF;
				}
			}
		}
	}
}


GLuint loadTexture(const std::string& path, bool flip = true) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format = 0;
		switch (nrComponents) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}

		/**
		* TODO: Part-3 create a texture map for an image
		*/
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

int main(void) {
	
	fillCheckers();

	GLFWwindow* window;
	if (!glfwInit()) { return -1; }
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(screenWidth, screenHeight, "program 2", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		return -1;
	}

	
	float coords[] = {
		// back
		-0.5f, -0.5f, -0.5f,  0.0f,0.0f,
		0.5f, -0.5f, -0.5f,   0.0f,1.0f,
		0.5f,  0.5f, -0.5f,   1.0f,0.0f,
		0.5f,  0.5f, -0.5f,   1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,0.0f,

		// front
		-0.5f, -0.5f,  0.5f,  0.0f,0.0f,
		0.5f, -0.5f,  0.5f,   0.0f,1.0f,
		0.5f,  0.5f,  0.5f,   1.0f,0.0f,
		0.5f,  0.5f,  0.5f,   1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,0.0f,

		// left
		-0.5f,  0.5f,  0.5f,  0.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f,1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,0.0f,

		// right
		0.5f,  0.5f,  0.5f,  0.0f,0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,0.0f,

		// bottom
		-0.5f, -0.5f, -0.5f, 0.0f,0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,0.0f,

		// top
		-0.5f,  0.5f, -0.5f,  0.0f,0.0f,
		0.5f,  0.5f, -0.5f,   0.0f,1.0f,
		0.5f,  0.5f,  0.5f,   1.0f,0.0f,
		0.5f,  0.5f,  0.5f,   1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,0.0f
	};


	glEnable(GL_DEPTH_TEST);
	Shader shader("vert.glsl", "frag.glsl");
	GLuint program = shader.id();



	GLuint VAO;
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords)  , &coords, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 320, 240, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, checkers);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//loadTexture("hazard.jpg", true);



	shader.use();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		processInput(window, shader);

		float timeValue = glfwGetTime() * .5;

		GLfloat scale[4][4] = {
			{ (float)cos(timeValue), 0.0f, (float)sin(timeValue), 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ (float)-sin(timeValue), 0.0f, (float)cos(timeValue), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		int scaleLocation = glGetUniformLocation(program, "scale");
		glUniformMatrix4fv(scaleLocation, 1, GL_TRUE, &scale[0][0]);


		/* Render here */

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(coords)/sizeof(float));

		/* Swap front and back and poll for io events */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}