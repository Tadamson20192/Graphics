#include "readFile.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>





int screenWidth = 640;
int screenHeight = 480;

int shadeState = 1;

float translateX = 0.0f;
float translateY = 0.0f;


std::vector<float> vertexes;

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

void loadShapes() {
	readFile object("text.txt");
	
	for (int i = 0; i < object.faces.size(); i++) {
		std::cout << "face " << i  << std::endl;
		
		
		vertexes.push_back(object.vertexes[object.faces[i].getP1()].getX());
		vertexes.push_back(object.vertexes[object.faces[i].getP1()].getY());
		vertexes.push_back(object.vertexes[object.faces[i].getP1()].getZ());
		if (shadeState == 1) {
			vertexes.push_back(object.vertexes[object.faces[i].getP1()].getNX());
			vertexes.push_back(object.vertexes[object.faces[i].getP1()].getNY());
			vertexes.push_back(object.vertexes[object.faces[i].getP1()].getNZ());
		}
		else {
			vertexes.push_back(object.faces[i].getNX());
			vertexes.push_back(object.faces[i].getNY());
			vertexes.push_back(object.faces[i].getNZ());
		}



		vertexes.push_back(object.vertexes[object.faces[i].getP2()].getX());
		vertexes.push_back(object.vertexes[object.faces[i].getP2()].getY());
		vertexes.push_back(object.vertexes[object.faces[i].getP2()].getZ());
		if (shadeState == 1) {
			vertexes.push_back(object.vertexes[object.faces[i].getP2()].getNX());
			vertexes.push_back(object.vertexes[object.faces[i].getP2()].getNY());
			vertexes.push_back(object.vertexes[object.faces[i].getP2()].getNZ());
		}
		else {
			vertexes.push_back(object.faces[i].getNX());
			vertexes.push_back(object.faces[i].getNY());
			vertexes.push_back(object.faces[i].getNZ());
		}

		vertexes.push_back(object.vertexes[object.faces[i].getP3()].getX());
		vertexes.push_back(object.vertexes[object.faces[i].getP3()].getY());
		vertexes.push_back(object.vertexes[object.faces[i].getP3()].getZ());
		if (shadeState == 1) {
			vertexes.push_back(object.vertexes[object.faces[i].getP3()].getNX());
			vertexes.push_back(object.vertexes[object.faces[i].getP3()].getNY());
			vertexes.push_back(object.vertexes[object.faces[i].getP3()].getNZ());
		}
		else {
			vertexes.push_back(object.faces[i].getNX());
			vertexes.push_back(object.faces[i].getNY());
			vertexes.push_back(object.faces[i].getNZ());
		}
	
	}

	
}


int main(void) {

	

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

	Shader shader("vert.glsl", "frag.glsl");
	
	glEnable(GL_DEPTH_TEST);
	GLuint program = shader.id();
	
	loadShapes();

	GLuint VAO;
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(GLfloat), &vertexes[0], GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	

	
	//delete the buffers and arrays to avoid runaway memory problems
	//glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(1, &VAO);

	shader.use();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		processInput(window, shader);

		float timeValue = glfwGetTime() * .1;
		
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
		glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

		/* Swap front and back and poll for io events */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}