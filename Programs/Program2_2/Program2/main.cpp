
// Author: Brandon O'Brien & Thomas Adamson
// CSCI_441 Lab 4
// Updated: 21 Feb 2018

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>



const int SCREEN_WIDTH = 940;
const int SCREEN_HEIGHT = 780;
float yPos = 0;
float xPos = 0;
float zPos = 0;
float scaler = 0;
float xAngle = 0;
float yAngle = 0;
float zAngle = 0;
float temp = 0;
float translateX = 0.0, translateY = 0.0, translateZ = 0.0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Shader &shader) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (yPos < 20) {
			yPos = yPos + .01;
			temp = 9;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (yPos > -20) {
			yPos = yPos - .01;
			temp = 9;
		}
	}

	//up down left right
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (translateY < 20) {
			translateY = translateY + .1;
			temp = 0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (translateY > -20) {
			translateY = translateY - .1;
			temp = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (translateX < 20) {
			translateX = translateX + .1;
			temp = 0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (translateX > -20) {
			translateX = translateX - .1;
			temp = 0;
		}
	}

	// in out
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
		if (translateY < 20) {
			zPos = zPos + .1;
			temp = 0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
		if (translateY > -20) {
			zPos = zPos - .1;
			temp = 0;
		}
	}

	// scale down
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		if (xPos < 20) {
			xPos = xPos + .001;
			yPos = yPos + .001;
			zPos = zPos + .001;
			temp = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		if (xPos > -20) {
			xPos = xPos - .001;
			yPos = yPos - .001;
			zPos = zPos - .001;
			temp = 1;
		}
	}
	//x rotate
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if (xPos < 20) {
			xPos = xPos + .1;
			temp = 2;
		}

	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (xPos > -20) {
			xPos = xPos - .1;
			temp = 2;
		}

	}
	//y rotate
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		if (yPos < 20) {
			yPos = yPos + .1;
			temp = 3;
		}

	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (yPos > -20) {
			yPos = yPos - .1;
			temp = 3;
		}

	}
	//z rotate
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
		if (zPos < 20) {
			zPos = zPos + .1;
			temp = 4;
		}

	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
		if (zPos > -20) {
			zPos = zPos - .1;
			temp = 4;
		}

	}

}

GLfloat* DetMatrix(float x) {

	GLfloat scale[4][4] = {
		{ 1.0F, 0.0f, 0.0f, xPos / 20 },
	{ 0.0f, 1.0F, 0.0f, yPos / 20 },
	{ 0.0f, 0.0f, 1.0F, zPos / 20 },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	GLfloat* address = &scale[0][0];
	return address;
}

GLfloat* scaleMatrix(float timeValue) {
	float scaleValue = sin(timeValue) + 1;
	GLfloat scale[4][4] = {
		{ xPos + 1.0f ,0.0f,0.0f,0.0f },
	{ 0.0f, yPos + 1.0f , 0.0f, 0.0f },
	{ 0.0f, 0.0f,  zPos + 1.0f , 0.0f },
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

	//float vertices[] = {
	////triangle 0
	//-0.25f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
	//.25f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f,//p1
	//0.0f, 0.25f, 0.5f,  0.0f, 1.0f, 1.0f,

	////triangle 1
	//-0.25f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,//p0
	//0.0f, 0.25f, 0.5f,  0.0f, 0.0f, 1.0f,//p5
	//-0.5f, 0.0f, 0.25f,  0.0f, 0.0f, 1.0f,//P11

	////triangle 2
	//0.0f, 0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//p5
	//-0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P11
	//0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//P4

	////triangle 3
	//0.0f, 0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//p5
	//0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
	//0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//P4

	////triangle 4
	//0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
	//0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
	//0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//P4

	////triangle 5
	//0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
	//0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9
	//0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8

	////triangle 6
	//0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
	//0.0f, -0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//P6
	//0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8

	////triangle 7
	//0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7
	//0.0f, -0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//P6
	//0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P8

	////triangle 8
	//0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7
	//0.0f, -0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//P6
	//-0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P10

	////triangle 9
	//0.0f, 0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//p7
	//-0.25f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,//P0
	//-0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P10

	////triangle 10
	//-0.5f, 0.0f, 0.25f,  0.0f, 1.0f, 0.0f,//P11
	//-0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,//P0
	//-0.5f, 0.0f, -0.25f,  0.0f, 1.0f, 0.0f,//P10

	////triangle 11
	//.25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
	//0.0f, 0.25f, 0.5f,  1.0f, 0.0f, 0.0f,//p5
	//0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9

	////triangle 12
	//.25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
	//0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8
	//0.5f, 0.0f, 0.25f,  1.0f, 0.0f, 0.0f,//P9

	////triangle 13
	//.25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
	//0.5f, 0.0f, -0.25f,  1.0f, 0.0f, 0.0f,//P8
	//0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7

	////triangle 14
	//.25f, 0.5f, 0.0f,  0.0f, 0.5f, 1.0f,//p1
	//0.0f, 0.25f, -0.5f,  1.0f, 1.0f, 0.0f,//p7
	//-0.25f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,//P0

	////triangle 15
	//-0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P10
	//-0.5f, 0.0f, 0.25f,  0.0f, 1.0f, 0.0f,//P11
	//-0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	////triangle 16
	//0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 1.0f,//P4
	//-0.5f, 0.0f, 0.25f,  0.0f, 1.0f, 0.0f,//P11
	//-0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	////triangle 17
	//0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
	//0.0f, -0.25f, 0.5f,  1.0f, 0.0f, 1.0f,//P4
	//-0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	////triangle 18
	//0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//p3
	//0.0f, -0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//P6
	//-0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2

	////triangle 19
	//-0.5f, 0.0f, -0.25f,  1.0f, 1.0f, 0.0f,//P10
	//0.0f, -0.25f, -0.5f,  1.0f, 0.0f, 0.0f,//P6
	//-0.25f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,//P2



	//};

	/* init the model */
	/* This is the cube  */

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,   0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,   0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,   0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
		(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

		xAngle = xAngle + xPos;
		yAngle = yAngle + yPos;
		zAngle = zAngle + zPos;

		float scaleValue = sin(timeValue) + 1;


		//tanslate
		if (temp == 0) {
			GLfloat scale[4][4] = {
				{ 1.0F, 0.0f, 0.0f, translateX / 20 },
			{ 0.0f, 1.0F, 0.0f, translateY / 20 },
			{ 0.0f, 0.0f, 1.0F, translateZ / 20 },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			int scaleLocation = glGetUniformLocation(program, "scale");
			glUniformMatrix4fv(scaleLocation, 1, GL_TRUE, &scale[0][0]);
		}

		//zoom
		else if (temp == 1) {
			GLfloat scale[4][4] = {
				{ xPos + 1.0f ,0.0f,0.0f,0.0f },
			{ 0.0f, yPos + 1.0f , 0.0f, 0.0f },
			{ 0.0f, 0.0f,  zPos + 1.0f , 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			int scaleLocation = glGetUniformLocation(program, "scale");
			glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);
		}

		// X rotate
		else if (temp == 2) {
			GLfloat scale[4][4] = {
				{ 1.0F, 0.0f, 0.0f, 0.0f },
			{ 0.0f, (float)cos(yAngle / 100), (float)-sin(yAngle / 100), 0.0f },
			{ 0.0f, (float)sin(yAngle / 100), (float)cos(yAngle / 100), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			int scaleLocation = glGetUniformLocation(program, "scale");
			glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);
		}

		// Y rotate
		else if (temp == 3) {
			GLfloat scale[4][4] = {
				{ (float)cos(yAngle / 100), 0.0f, (float)sin(yAngle / 100), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ (float)-sin(yAngle / 100), 0.0f, (float)cos(yAngle / 100), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			int scaleLocation = glGetUniformLocation(program, "scale");
			glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);
		}

		// z rotate
		else if (temp == 4) {
			GLfloat scale[4][4] = {
				{ (float)cos(yAngle / 100), (float)-sin(yAngle / 100), 0.0f, 0.0f },
			{ (float)sin(yAngle / 100), (float)cos(yAngle / 100), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			int scaleLocation = glGetUniformLocation(program, "scale");
			glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);
		}

		else if (temp == 9) {
			GLfloat scale[4][4] = {
				{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, yPos },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			};
			int scaleLocation = glGetUniformLocation(program, "scale");
			glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, &scale[0][0]);
		}


		/* Render here */
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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