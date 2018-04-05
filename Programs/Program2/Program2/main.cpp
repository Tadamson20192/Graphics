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
#include "model.h"




int screenWidth = 640;
int screenHeight = 480;

int shadeState = 2;
int viewState = 1;

float translateX = 0.0f;
float translateY = 0.0f;
float sensitivity = .01;
float xPos = -.1, yPos = .1, zPos = -.8; //center of box
float xLook = 0, yLook = .0, zLook = 1;// point box is looking at


std::vector<float> cubeVec;
std::vector<float> dinoVec;
Matrix view;
Matrix projection;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	screenWidth = height;
	screenHeight = width;
}

bool isPressed(GLFWwindow *window, int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isReleased(GLFWwindow *window, int key) {
	return glfwGetKey(window, key) == GLFW_RELEASE;
}

Matrix processModel(const Matrix& model, GLFWwindow *window) {
	Matrix trans;

	const float ROT = 1 * sensitivity;
	const float SCALE = .05*sensitivity;
	const float TRANS = .01*sensitivity;

	// ROTATE
	if (isPressed(window, GLFW_KEY_U)) { trans.rotate_x(-ROT); }
	else if (isPressed(window, GLFW_KEY_I)) { trans.rotate_x(ROT); }
	else if (isPressed(window, GLFW_KEY_O)) { 
		double theta_radians = -ROT * 3.14156 / 180.0;
		trans.rotate_y(-ROT); 
		xLook = (xLook * cos(theta_radians)) + (zLook * sin(theta_radians));
		zLook = (xLook * sin(theta_radians)) + (zLook * cos(theta_radians));
	}
	else if (isPressed(window, GLFW_KEY_P)) { 
		double theta_radians = -ROT * 3.14156 / 180.0;
		trans.rotate_y(ROT); 
		xLook = (xLook * cos(theta_radians)) + (zLook * sin(theta_radians));
		zLook = (xLook * sin(theta_radians)) + (zLook * cos(theta_radians));
	}
	else if (isPressed(window, '[')) { trans.rotate_z(-ROT); }
	else if (isPressed(window, ']')) { trans.rotate_z(ROT); }
	// SCALE
	else if (isPressed(window, '-')) { trans.scale(1 - SCALE, 1 - SCALE, 1 - SCALE); }
	else if (isPressed(window, '=')) { trans.scale(1 + SCALE, 1 + SCALE, 1 + SCALE); }
	// TRANSLATE
	else if (isPressed(window, GLFW_KEY_UP)) { 
		trans.translate(0, 0, TRANS); 
		zPos = zPos + TRANS;
	}
	else if (isPressed(window, GLFW_KEY_DOWN)) { 
		trans.translate(0, 0, -TRANS); 
		zPos = zPos - TRANS;
	}
	else if (isPressed(window, GLFW_KEY_LEFT)) { 
		trans.translate(-TRANS, 0, 0); 
		xPos = xPos - TRANS;
	}
	else if (isPressed(window, GLFW_KEY_RIGHT)) { 
		trans.translate(TRANS, 0, 0); 
		xPos = xPos + TRANS;
	}
	else if (isPressed(window, ',')) { trans.translate(0, 0, TRANS); }
	else if (isPressed(window, '.')) { trans.translate(0, 0, -TRANS); }
	else if (isPressed(window, GLFW_KEY_B)) {view.birdView();}
	else if (isPressed(window, GLFW_KEY_F)) {view.fpv(xPos, yPos, zPos, xLook, yLook, zLook);}
	return trans * model;
}

void processInput(Matrix& model, GLFWwindow *window) {
	if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
		glfwSetWindowShouldClose(window, true);
	}
	model = processModel(model, window);
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

void loadShapes(readFile object, std::vector<float> &vertexes) {


	for (int i = 0; i < object.faces.size(); i++) {
		std::cout << "face " << i << std::endl;
		std::cout << "shading: " << shadeState << std::endl;

		vertexes.push_back(object.vertexes[object.faces[i].getP1()].getX());
		vertexes.push_back(object.vertexes[object.faces[i].getP1()].getY());
		vertexes.push_back(object.vertexes[object.faces[i].getP1()].getZ());
		if (shadeState == 1) {
			std::cout << "normal " << object.vertexes[object.faces[i].getP1()].getNX() << " " << object.vertexes[object.faces[i].getP1()].getNY() << " " << object.vertexes[object.faces[i].getP1()].getNZ() << std::endl;
			vertexes.push_back(object.vertexes[object.faces[i].getP1()].getNX());
			vertexes.push_back(object.vertexes[object.faces[i].getP1()].getNY());
			vertexes.push_back(object.vertexes[object.faces[i].getP1()].getNZ());
		}
		else {
			std::cout << "normal " << object.faces[i].getNX() << " " << object.faces[i].getNY() << " " << object.faces[i].getNZ() << std::endl;
			vertexes.push_back(object.faces[i].getNX());
			vertexes.push_back(object.faces[i].getNY());
			vertexes.push_back(object.faces[i].getNZ());
		}



		vertexes.push_back(object.vertexes[object.faces[i].getP2()].getX());
		vertexes.push_back(object.vertexes[object.faces[i].getP2()].getY());
		vertexes.push_back(object.vertexes[object.faces[i].getP2()].getZ());
		if (shadeState == 1) {
			std::cout << "normal " << object.vertexes[object.faces[i].getP2()].getNX() << " " << object.vertexes[object.faces[i].getP2()].getNY() << " " << object.vertexes[object.faces[i].getP2()].getNZ() << std::endl;
			vertexes.push_back(object.vertexes[object.faces[i].getP2()].getNX());
			vertexes.push_back(object.vertexes[object.faces[i].getP2()].getNY());
			vertexes.push_back(object.vertexes[object.faces[i].getP2()].getNZ());
		}
		else {
			std::cout << "normal " << object.faces[i].getNX() << " " << object.faces[i].getNY() << " " << object.faces[i].getNZ() << std::endl;
			vertexes.push_back(object.faces[i].getNX());
			vertexes.push_back(object.faces[i].getNY());
			vertexes.push_back(object.faces[i].getNZ());
		}

		vertexes.push_back(object.vertexes[object.faces[i].getP3()].getX());
		vertexes.push_back(object.vertexes[object.faces[i].getP3()].getY());
		vertexes.push_back(object.vertexes[object.faces[i].getP3()].getZ());
		if (shadeState == 1) {
			std::cout << "normal " << object.vertexes[object.faces[i].getP3()].getNX() << " " << object.vertexes[object.faces[i].getP3()].getNY() << " " << object.vertexes[object.faces[i].getP3()].getNZ() << std::endl;
			vertexes.push_back(object.vertexes[object.faces[i].getP3()].getNX());
			vertexes.push_back(object.vertexes[object.faces[i].getP3()].getNY());
			vertexes.push_back(object.vertexes[object.faces[i].getP3()].getNZ());
		}
		else {
			std::cout << "normal " << object.faces[i].getNX() << " " << object.faces[i].getNY() << " " << object.faces[i].getNZ() << std::endl;
			vertexes.push_back(object.faces[i].getNX());
			vertexes.push_back(object.faces[i].getNY());
			vertexes.push_back(object.faces[i].getNZ());
		}

	}


}

void changeToBird() {
	view.birdView();
}
void changeToFPV() {
	view.birdView();
}

int main(void) {
	int temp;
	std::cout << "Choose 1 for smooth shading or 2 for flat shading" << std::endl;
	std::cin >> temp;
	shadeState = temp;


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

	projection.perspective(-1,1,-1,1,-1,1);
	readFile dino("maze.txt", .23);
	loadShapes(dino, dinoVec);
	Model obj2(dinoVec,Shader("vert.glsl", "frag.glsl"));
	obj2.shader.use();
	obj2.changeColor(0, 1, 0);
	obj2.model.rotate_y(-90);
	obj2.render(projection, view);

	readFile cube("cube.txt", .1);
	loadShapes(cube, cubeVec);
	Model obj(cubeVec, Shader("vert.glsl", "frag.glsl"));
	obj.changeColor(1, 0, 0);
	obj.render(projection, view);
	obj.model.translate(-.1, .05, -.85);
	glEnable(GL_DEPTH_TEST);
	
	

	

	//GLuint VAO;
	//GLuint VBO;
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, cubeVec.size() * sizeof(GLfloat), &cubeVec[0], GL_STATIC_DRAW);

	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
	//	(void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
	//	(void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);




	//delete the buffers and arrays to avoid runaway memory problems
	//glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(1, &VAO);

	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		processInput(obj.model, window);

		
		
		
		


		/* Render here */

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		obj.render(projection, view);
		obj2.render(projection, view);
		//obj2.render();

		/* Swap front and back and poll for io events */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}