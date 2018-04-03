
#ifndef Face_H
#define Face_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <cmath>


class Face {

public:
	int p1, p2, p3;
	int n1, n2, n3;
	int t1, t2, t3;
	float normal;

	Face(int pp1, int pp2, int pp3) {
		p1 = pp1;
		p2 = pp2;
		p3 = pp3;
		
	}

	Face(int pp1, int tt1, int nn1, int pp2, int tt2, int nn2, int pp3, int tt3, int nn3) {
		p1 = pp1;
		n1 = nn1;
		t1 = tt1;
		
		p2 = pp2;
		n2 = nn2;
		t2 = tt2;
		
		
		p3 = pp3;
		n3 = nn3;
		t3 = tt3;
	}

	int getP1(void) { return p1; }
	int getP2(void) { return p2; }
	int getP3(void) { return p3; }

	void setNorm(float x, float y, float z) {
		n1 = x;
		n2 = y;
		n3 = z;
		
	}

	float getNX() { return n1; }
	float getNY() { return n2; }
	float getNZ() { return n3; }

};

#endif