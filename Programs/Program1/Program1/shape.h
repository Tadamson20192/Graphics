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
	float triangle[18] = {
	0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
	
	};




	
	Triangle(float xx, float yy, int zz) {

		triangle[0] = 0.0f + xx;
		triangle[1] = 0.25 + yy;
		triangle[6] = -0.25f + xx;
		triangle[7] = -0.25f + yy;
		triangle[12] = 0.25f + xx;
		triangle[13] = -0.25f + yy;
	
	}

	int getX() const {
	
		return x;
	}
	
	void translate(float x, float y) {
		
		
		triangle[0] = triangle[0] + x;
		triangle[1] = triangle[1] + y;
		triangle[6] = triangle[6] + x;
		triangle[7] = triangle[7] + y;
		triangle[12] = triangle[12] + x;
		triangle[13] = triangle[13] + y;
	}

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

	

};
class Rectangle {



private:
	int x, y, z;

public:
	float triangle[18] = {
		0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f,

	};




	Rectangle(int xx, int yy, int zz) {
		x = 1;


	}

	int getX() const {

		return x;
	}

	void translate(float x, float y) {


		triangle[0] = triangle[0] + x;
		triangle[1] = triangle[1] + y;
		triangle[6] = triangle[6] + x;
		triangle[7] = triangle[7] + y;
		triangle[12] = triangle[12] + x;
		triangle[13] = triangle[13] + y;
	}



};

#endif