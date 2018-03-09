#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include "shape.h"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAXSTATE = 1;
float yPos = 0;
int state = 0;

int selected = -1;

std::vector<Triangle> triangles;    // declares a vector of integers


GLuint VAO;


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void selectObject(float x, float y) {

	for (int i = 0; i < triangles.size(); i++) {



		if (x > triangles[i].xLowerBoundary() && x < triangles[i].xUpperBoundary() && y > triangles[i].yLowerBoundary() && y <  triangles[i].yUpperBoundary()) {

			selected = i;

		}

	}
	std::cout << "You selected shape #" << selected << std::endl;
}



void drawShape() {






	for (int i = 0; i < triangles.size(); i++) {
		//triangles[i]

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangles[i].triangle), triangles[i].triangle, GL_STATIC_DRAW);



		// describe vertex layout

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(triangles[i].triangle));

		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}








}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = (xpos / (SCREEN_WIDTH / 2)) - 1;
		ypos = -1 * ((ypos / (SCREEN_HEIGHT / 2)) - 1);
		std::cout << xpos << ", " << ypos << std::endl;
		std::cout << "Mouse clicked " << std::endl;

		if (state == 0) {
			triangles.push_back(Triangle((float)xpos, (float)ypos, 1));
			drawShape();

		}
		else if (state == 1) {
			selectObject((float)xpos, (float)ypos);

		}
		//t1.translate();


	}

}

void processInput(GLFWwindow *window, Shader &shader) {




	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (yPos < 20) {
			yPos = yPos + .1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (yPos > -20) {
			yPos = yPos - .1;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (state < MAXSTATE) {
			state = state + 1;
		}
		else {
			state = 0;
		}
		std::cout << "State change to: " << state << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (selected > -1) {
			triangles[selected].translate(0, .001);
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (selected > -1) {
			triangles[selected].translate(0, -.001);
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (selected > -1) {
			triangles[selected].translate(-.001, 0);
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (selected > -1) {
			triangles[selected].translate(.001, 0);
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
}




void errorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(void) {
	GLFWwindow* window;

	glfwSetErrorCallback(errorCallback);

	/* Initialize the library */
	if (!glfwInit()) { return -1; }

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 4", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// tell glfw what to do on resize
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// init glad
	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		return -1;
	}



	/*float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,

	};*/



	// copy vertex data


	// create the shaders

	Shader shader("vert.glsl", "frag.glsl");
	// setup the textures
	shader.use();

	// and use z-buffering
	glEnable(GL_DEPTH_TEST);




	GLuint program = shader.id();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		// process input
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		processInput(window, shader);

		//int scaleLocation = glGetUniformLocation(program, "scale");
		//glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);


		/* Render here */

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		drawShape();


		/* Swap front and back and poll for io events */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();



	return 0;
}