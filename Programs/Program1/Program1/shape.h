#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>


class Triangle{



private:
	int x, y, z;
	

public:
	//keeps track if triangle is in a group or not
	int group = -1;


	//The vertices for each triangle object
	float triangle[18] = {
	0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
	
	};




	//Generic Triangle construcor
	Triangle(float xx, float yy, int z) {

		triangle[0] = 0.0f + xx;
		triangle[1] = 0.25 + yy;
		triangle[6] = -0.25f + xx;
		triangle[7] = -0.25f + yy;
		triangle[12] = 0.25f + xx;
		triangle[13] = -0.25f + yy;
	
		group = z;
	}
	//Triangle Constructor where User can set each individual vertex
	Triangle(float x1, float x2, float x3, float y1, float y2, float y3, int z) {
	
		triangle[0] = x1;
		triangle[1] = y1;
		triangle[6] = x2;
		triangle[7] = y2;
		triangle[12] = x3;
		triangle[13] = y3;
	
		group = z;
	}


	//Translates the triangle left right or up and down
	void translate(float x, float y) {
		
		
		triangle[0] = triangle[0] + x;
		triangle[1] = triangle[1] + y;
		triangle[6] = triangle[6] + x;
		triangle[7] = triangle[7] + y;
		triangle[12] = triangle[12] + x;
		triangle[13] = triangle[13] + y;
	}

	//translates the points around the z axis, left or right based on input
	void rotate(float theta){
	
		triangle[0] = (triangle[0]* cos(theta)- (triangle[1] * sin(theta)));
		triangle[1] = (triangle[0] * sin(theta) + (triangle[1] * cos(theta)));
		triangle[6] = (triangle[6] * cos(theta) - (triangle[7] * sin(theta)));
		triangle[7] = (triangle[6] * sin(theta) + (triangle[7] * cos(theta)));
		triangle[12] = (triangle[12] * cos(theta) - (triangle[13] * sin(theta)));
		triangle[13] = (triangle[12] * sin(theta) + (triangle[13] * cos(theta)));
		
	
	}

	//scales object based on input
	void scale(float scale) {
		triangle[0] = triangle[0] * scale;
		triangle[1] = triangle[1] * scale;
		triangle[6] = triangle[6] * scale;
		triangle[7] = triangle[7] * scale;
		triangle[12] = triangle[12] * scale;
		triangle[13] = triangle[13] * scale;
		
	}

	//changes triangle color to red
	void changeRed(){
		triangle[3] = 1.0f;
		triangle[4] = 0.0f;
		triangle[5] = 0.0f;

		triangle[9] = 1.0f;
		triangle[10] = 0.0f;
		triangle[11] = 0.0f;

		triangle[12] = 1.0f;
		triangle[13] = 0.0f;
		triangle[14] = 0.0f;
	
	}
	//changes triangle color to blue
	void changeBlue() {
		triangle[3] = 0.0f;
		triangle[4] = 0.0f;
		triangle[5] = 1.0f;

		triangle[9] = 0.0f;
		triangle[10] = 0.0f;
		triangle[11] = 1.0f;

		triangle[12] = 0.0f;
		triangle[13] = 0.0f;
		triangle[14] = 1.0f;

	}
	//changes triangle color to green
	void changeGreen() {
		triangle[3] = 0.0f;
		triangle[4] = 1.0f;
		triangle[5] = 0.0f;

		triangle[9] = 0.0f;
		triangle[10] = 1.0f;
		triangle[11] = 0.0f;

		triangle[12] = 0.0f;
		triangle[13] = 1.0f;
		triangle[14] = 0.0f;

	}
	//Set of methods encapsulate an object in min/max coordinates for easier triangle selection
	float xLowerBoundary() {
		float min = fmin(triangle[0], fmin(triangle[6], triangle[12]));
		return min;
	}

	float xUpperBoundary() {
		return fmax(triangle[0], fmax(triangle[6], triangle[12]));
	}

	float yLowerBoundary() {
		return fmin(triangle[1], fmin(triangle[7], triangle[13]));
	}

	float yUpperBoundary() {
		return fmax(triangle[1], fmax(triangle[7], triangle[13]));
	}
	//returns the group number for a shape -1 means not in a group
	int getGroup() {
	
		return group;

	}
	//sets the group number for a triangle
	void setGroup(int groupNumber) {
		group = groupNumber;
	}


};


#endif