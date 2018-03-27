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


int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int groupCounter = 1; 
float yPos = 0;
int state = 0;
int shape = 0; //0 = triangle, 1 = square
bool group = false;
int selected = -1;
int groupSelected = -1;
float translateView[4][4] = {
	{1,0,0,0},
	{0,1,0,0},
	{0,0,1,0},
	{0,0,0,1} };

std::vector<int> grouping;
std::vector<Triangle> triangles;    //declares a vector of shapes

//variables for mouseclicker Method
double xpos, ypos;
double startX = 0;
double startY = 0;
double changeX;
double changeY;
bool isHeld = false;

GLuint VAO;

//sets new screen height and width based on new dimensions
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	
	SCREEN_HEIGHT = height;
	SCREEN_WIDTH = width;
}

//creates a triangle object and adds it to the vector
void drawTriangle(float x, float y) {

	triangles.push_back(Triangle(x, y, -1));

}

// creates two triangle objects, groups them together and adds them to the vector
void drawSquare(float x, float y) {
	float x1 = x + .25;
	float x2 = x - .25;
	float y1 = y + .25;
	float y2 = y - .25;

	triangles.push_back(Triangle(x2, x2, x1, y2, y1, y1, groupCounter));
	triangles.push_back(Triangle(x2, x1, x1, y2, y2, y1, groupCounter));
	groupCounter = groupCounter + 1;
}

//Class which determines if a triangle was clicked on
bool selectObject(float x, float y) {
	bool flag = false;//flag for whether not a triangle was clicked on
	//loop checking each triangle in the vector
	for (int i = 0; i < triangles.size(); i++) {


		//checking if mouse click coordinates are withing bounds for indiviual triangles
		if (x > triangles[i].xLowerBoundary() && x < triangles[i].xUpperBoundary() && y > triangles[i].yLowerBoundary() && y <  triangles[i].yUpperBoundary()) {
			flag = true;
			selected = i;
			std::cout << "Shape is in group" << triangles[i].getGroup() << std::endl;
			//checks if triangle selected is in a group or not
			if (triangles[i].getGroup() > 0) {
				group = true;//sets flag if triangle is part of a group
				groupSelected = triangles[i].getGroup();
				std::cout << "You selected group #" << groupSelected << std::endl;
			}
			else { group = false; }
		}

	}
	std::cout << "You selected shape #" << selected << std::endl;
	return flag;
}

//gathers selected triangles into the vector grouping
void groupShapes(float x, float y) {
	if (selectObject(x,y)) {//checks if triangle was clicked on
		
		if (group) {//triangle is part of a group add entire group to grouping
			for (int i = 0; i < triangles.size(); i++) {
				if (triangles[i].getGroup() == groupSelected) {
					grouping.push_back(i);
				}
			}
		}
		else {//add individual triangle to group
			
			grouping.push_back(selected);
			
		}
	}
}

void ungroup() {//takes each triangle in grouping and sets the group to -1 ungrouping it
	for (int i = 0; i < grouping.size(); i++) {
		triangles[grouping[i]].setGroup(-1);
	}
}
void formGroup() {//takes all of the triangles in grouping and set there group to the same
	std::cout << "Grouping Size: " << grouping.size() << std::endl;

	for (int i = 0; i < grouping.size(); i++) {
		triangles[grouping[i]].setGroup(groupCounter);
	}
	groupCounter = groupCounter + 1; 
}


//creates a bunch of triangles and forms them in a circle
void drawCircle(float x, float y, float radius) {
	int i =0;
	int triangleAmount = 20; //# of triangles used to draw circle
	float centerX = x;
	float centerY = y;
	float twicePi = 2.0f * 3.1415;
	//setting the start vertices
	float theta = i * twicePi / triangleAmount;
	float x1 = x + (radius * cos(theta));
	float y1 = y + (radius * sin(theta));
		
	for (i = 0; i <= triangleAmount; i++) {
		//setting the second point for the triangle 		
		theta = i * twicePi / triangleAmount;
		float x2 = x + (radius * cos(theta));
		float y2 = y + (radius * sin(theta));
		triangles.push_back(Triangle(x, x1, x2, y, y1, y2, groupCounter));
		//sets the second point to first point
		x1 = x2;
		y1 = y2;
	}
	groupCounter = groupCounter + 1;
}


//runs through the triangles vector and draws each individual triangle with its own array and buffer
void drawShape() {
	
	for (int i = 0; i < triangles.size(); i++) {
		//triangles[i]

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangles[i].triangle), triangles[i].triangle, GL_STATIC_DRAW);


		
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
		//delete the buffers and arrays to avoid runaway memory problems
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}








}



//Set of methods to do modifications to entire groups
void translateGroups(float x, float y){
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getGroup() == groupSelected) {
			triangles[i].translate(x, y);
		}
	}
}

void scaleGroups(float scale) {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getGroup() == groupSelected) {
			triangles[i].scale(scale);
		}
	}
}

void rotateGroups(float theta) {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getGroup() == groupSelected) {
			triangles[i].rotate(theta);
		}
	}

}

void changeGroupsRed() {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getGroup() == groupSelected) {
			triangles[i].changeRed();
		}
	}
}

void changeGroupsBlue() {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getGroup() == groupSelected) {
			triangles[i].changeBlue();
		}
	}
}

void changeGroupsGreen() {
	for (int i = 0; i < triangles.size(); i++) {
		if (triangles[i].getGroup() == groupSelected) {
			triangles[i].changeGreen();
		}
	}
}
//mouse listener method
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if mouse clicked
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		
		glfwGetCursorPos(window, &xpos, &ypos);
		
		std::cout << xpos << ", " << ypos << std::endl;
		std::cout << "Mouse clicked " << std::endl;

		//determines if the user is stamping, modifying, viewing or grouping
		if (state == 0) {//stamp state
			xpos = (xpos / (SCREEN_WIDTH / 2)) - 1;
			ypos = -1 * ((ypos / (SCREEN_HEIGHT / 2)) - 1);
			xpos = xpos * (1 / translateView[0][0]) - translateView[0][3];//modify coordinates based on the view
			ypos = ypos * (1 / translateView[1][1]) - translateView[1][3];
			if (shape == 0) {//draws a triangle
				drawTriangle((float)xpos, (float)ypos);
			}
			if (shape == 1) {//draws a square
				drawSquare((float)xpos, (float)ypos);
			}
			if (shape == 2) {//draws a circle
				drawCircle((float)xpos, (float)ypos, 0.25f);
			}

			drawShape();
			xpos = 0; 
			ypos = 0;
		}
		else if (state == 1) {//modify state
			xpos = (xpos / (SCREEN_WIDTH / 2)) - 1;
			ypos = -1 * ((ypos / (SCREEN_HEIGHT / 2)) - 1);
			selectObject((float)xpos, (float)ypos);//checks if object was selected based on coordinates
			xpos = 0;
			ypos = 0;
		}
		else if (state == 2) {//sets mouse clicked location
			isHeld = true;
			startX = xpos;
			startY = ypos;
			
		}
		else if (state == 3) {//checks to add shape to grouping
			xpos = (xpos / (SCREEN_WIDTH / 2)) - 1;
			ypos = -1 * ((ypos / (SCREEN_HEIGHT / 2)) - 1);
			groupShapes(xpos, ypos);
		}
		


	}
	
	//if the mouse was dragged get end coordinates and calculate amount moved and translate view based on movement
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && isHeld) {
		isHeld = false;//set release
		bool right = false;
		bool up = false;
		if (state == 2) {
			glfwGetCursorPos(window, &xpos, &ypos);
			
			if (startX < xpos) { right = true; }//determine direction mouse was moved
			if (startY > ypos) { up = true; }
			changeX = abs(abs(startX) - abs(xpos));//determine x and y changes
			changeY = abs(abs(startY) - abs(ypos));
			startX = xpos;
			startY = ypos;
			std::cout << "Mouse dragged : " << changeX << "in x direction" << changeY << "in Y direction" << std::endl;
						
			changeX = (changeX / (SCREEN_WIDTH / 2));//convert to viewport coordinates[-1,1]
			changeY =  ((changeY / (SCREEN_HEIGHT / 2)));
						
			//moves view based on mouse movement
			if (right) {translateView[0][3] = translateView[0][3] - (.5 * changeX); }
			else { translateView[0][3] = translateView[0][3] + (.5 * changeX); }
			if (up) { translateView[1][3] = translateView[1][3] - (.5 * changeY); }
			else { translateView[1][3] = translateView[1][3] + (.5 * changeY); }
			
			//sets everything back to 0
			xpos = 0;
			ypos = 0;
			startX = 0;
			startY = 0;
			changeX = 0;
			changeY = 0;
		}
	}


}

//keyboard inputs
void processInput(GLFWwindow *window, Shader &shader) {
	//For view state
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		if (state == 2 && translateView[0][0] < 5) {
			translateView[0][0] = translateView[0][0] + .001;
			translateView[1][1] = translateView[1][1] + .001;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		if (state == 2 && translateView[0][0] > 0) {
			translateView[0][0] = translateView[0][0] - .001;
			translateView[1][1] = translateView[1][1] - .001;
		} 
	}
	//Close window on escape prss
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//For changing the shape that is stamped in stamp mode
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		shape = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		shape = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		shape = 2;
	}

	//change to group state
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		state = 0;
		std::cout << "State change to: " << state << std::endl;
	}
		
	//changes state to select state where user can selct modify and transform objects
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		state = 1;
	
		std::cout << "State change to: " << state << std::endl;
	}

	//change to view state, user can edit view using mouse and - +
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		state = 2;
	
	std::cout << "State change to: " << state << std::endl;
	}

	//change to group state, user can group and ungroup objects
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		state = 3;

		std::cout << "State change to: " << state << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {

		ungroup();
		std::cout << "State change to: " << state << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {

		formGroup();
		if (!grouping.empty()) { grouping.clear(); }
		
	}
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
	
		//triangles.erase(triangles.begin() + selected);
	
	}

	//for tranlating an object in select state
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { translateGroups(0, .001); }
			else {triangles[selected].translate(0, .001); }
			
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { translateGroups(0, -.001); }
			else { triangles[selected].translate(0, -.001); }
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { translateGroups(-.001, 0); }
			else { triangles[selected].translate(-.001, 0); }
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { translateGroups(.001, 0); }
			else { triangles[selected].translate(.001, 0); }
			
			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { scaleGroups(1.001); }
			else { triangles[selected].scale(1.001); }

			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { scaleGroups(.999); }
			else { triangles[selected].scale(.999); }

			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { rotateGroups(.001); }
			else { triangles[selected].rotate(.001); }

			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { rotateGroups(-.001); }
			else { triangles[selected].rotate(-.001); }

			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { changeGroupsRed(); }
			else { triangles[selected].changeRed(); }

			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { changeGroupsGreen(); }
			else { triangles[selected].changeGreen(); }

			drawShape();
		}
		else {
			std::cout << "Please select an object" << std::endl;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (selected > -1) {
			if (group) { changeGroupsBlue(); }
			else { triangles[selected].changeBlue(); }

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

	


	Shader shader("vert.glsl", "frag.glsl");
	shader.use();
	glEnable(GL_DEPTH_TEST);
	GLuint program = shader.id();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		// process input
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		processInput(window, shader);

		//send translation matrix for view mode
		int scaleLocation = glGetUniformLocation(program, "scale");
		glUniformMatrix4fv(scaleLocation, 1, GL_TRUE, &translateView[0][0]);


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