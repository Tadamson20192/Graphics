#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>



const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
float yPos = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
}

GLfloat* scaleMatrix(float timeValue) {
	float scaleValue = sin(timeValue) + 1;
	GLfloat scale[4][4] = {
		{ scaleValue,0.0f,0.0f,0.0f },
		{ 0.0f, scaleValue, 0.0f, 0.0f },
		{ 0.0f, 0.0f,  scaleValue, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	GLfloat* address = &scale[0][0];
	return address;
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
	/*
	float vertices[] = {
		//triangle 0
		-0.25f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
		.25f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f,//p1
		0.0f, 0.25f, 0.5f,  0.0f, 1.0f, 1.0f,

		//triangle 1
		-0.25f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,//p0
		0.0f, 0.25f, 0.5f,  0.0f, 0.0f, 1.0f,//p5
		-0.5f, 0.0f, 0.25f,  0.0f, 0.0f, 1.0f,//P11

		//triangle 2
		0.0f, 0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//p5
		-0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P11
		0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//P4

		//triangle 3
		0.0f, 0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//p5
		0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
		0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//P4

		//triangle 4
		0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
		0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
		0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//P4

		//triangle 5
		0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
		0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
		0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8

		//triangle 6
		0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
		0.0f, -0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//P6
		0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8

		//triangle 7
		0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7
		0.0f, -0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//P6
		0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P8

		//triangle 8
		0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7
		0.0f, -0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//P6
		-0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P10

		//triangle 9
		0.0f, 0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//p7
		-0.25f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,//P0
		-0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P10

		//triangle 10
		-0.5f, 0.0f, 0.25f,  0.0f, 1.0f, 0.0f,//P11
		-0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,//P0
		-0.5f, 0.0f, -0.25f,  0.0f, 1.0f, 0.0f,//P10

		//triangle 11
		.25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
		0.0f, 0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//p5
		0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
		
		//triangle 12
		.25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
		0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8
		0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9

		//triangle 13
	   .25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
	   0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8
	   0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7

	   //triangle 14
	   .25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
	   0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8
	   -0.25f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,//P0

	   	//triangle 15
		-0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P10
		-0.5f, 0.0f, 0.25f,  0.0f, 1.0f, 0.0f,//P11
	   -0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	   //triangle 16
	   0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 1.0f,//P4
		-0.5f, 0.0f, 0.25f,  0.0f, 1.0f, 0.0f,//P11
	   -0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	   //triangle 17
	   0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
		0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 1.0f,//P4
	   -0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	   //triangle 18
	   0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
	   0.0f, -0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//P6
	   -0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	   //triangle 19
		-0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P10
		0.0f, -0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//P6
	   -0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2



	};
	*/
	/* init the model */
	/* This is the cube  */
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
	};
	
	// copy vertex data
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// describe vertex layout
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		processInput(window, shader);

		float timeValue = glfwGetTime();
		GLfloat* address = scaleMatrix(timeValue);

		float scaleValue = sin(timeValue) + 1;
		GLfloat scale[4][4] = {
		{ 1.0F,0.0f,0.0f,0.0f },
		{ 0.0f, 1.0F, 0.0f, yPos/20 },
		{ 0.0f, 0.0f,  0.0F, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		int scaleLocation = glGetUniformLocation(program, "scale");
		glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);


		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		shader.use();



		// render the cube
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));

		/* Swap front and back and poll for io events */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}