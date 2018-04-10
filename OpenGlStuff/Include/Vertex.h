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
	float nx = 0, ny = 0, nz = 0;
	int normCount = 0;


	Vertex(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
		std::cout << "normal start " << nx << " " << ny << " " << nz << std::endl;
		std::cout << "count " << normCount << std::endl;
	}

	float getX(void) { return x; }
	float getY(void) { return y; }
	float getZ(void) { return z; }
	float getNX(void) { return nx; }
	float getNY(void) { return ny; }
	float getNZ(void) { return nz; }

	void adjustNormal(float xxx, float yyy, float zzz) {
		nx = nx + xxx;
		ny = ny + yyy;
		nz = nz + zzz;
		normCount = normCount + 1;

	}

	void finishNormal() {
		if (normCount > 0) {
			
			nx = nx / normCount;
			ny = ny / normCount;
			nz = nz / normCount;
		}
	}
};

#endif