#ifndef Vertex_H
#define Vertex_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <cmath>


class Vertex {

public:
	float x, y, z;
	float nx, ny, nz;
	int normCount = 0;
	

	Vertex(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
		
	}

	float getX(void){ return x; }
	float getY(void) { return y; }
	float getZ(void) { return z; }
	float getNX(void) { return nx; }
	float getNY(void) { return ny; }
	float getNZ(void) { return nz; }

	void adjustNormal(float xx, float yy, float zz) {
		x = x + xx;
		y = y + yy;
		z = z + zz; 
		normCount = normCount + 1;
	
	}

	void finishNormal() {
		if (normCount > 0) {
			x = x / normCount;
			y = y / normCount;
			z = z / normCount;
		}
	}
};

#endif