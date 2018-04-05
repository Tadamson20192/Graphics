#pragma once
#ifndef readFile_H
#define readFile_H

#include "Vertex.h"
#include "Face.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

class readFile {

public:
	std::vector<Vertex> vertexes;
	std::vector<Face> faces;
	std::vector<float> coords;
	

	readFile(std::string file) {

		std::string line;
		//string v, valuesX[8], valuesY[8], valuesZ[8];
		int n = 0;
		char v, eat1, eat2, eat3;
		float x, y, z;


		std::ifstream myfile(file);
		while (!myfile.eof())
		{
			std::cout << "I made it here" << std::endl;
			getline(myfile, line);
			if (line[0] == 'v')
			{
				std::istringstream iss(line);
				iss >> v >> x >> y >> z;
				vertexes.push_back(Vertex(x, y, z));
				std::cout << x << " " << y << " " << z << std::endl;
			}//if Vector
			else if (line[0] == 'f')
			{
				int p1, p2, p3;

				std::istringstream iss(line);
				iss >> v >> p1 >> p2 >> p3;
				faces.push_back(Face(p1 - 1, p2 - 1, p3 - 1));
				std::cout << p1 << " " << p2 << " " << p3 << std::endl;


				//std::vector<int> faceValues;
				//std::string substr;
				//std::stringstream ss(line);
				//std::getline(ss, substr, ' ');
				//
				//int counter = 1;
				//while (ss.good())
				//{	
				//						
				//	int temp;
				//	std::getline(ss, substr, '/');
				//	std::cout << substr << std::endl;
				//	ss >> temp;
				//	faceValues.push_back(temp);
				//	counter = counter + 1;
				//}
				//int v1 = faceValues[0];
				//int t1 = faceValues[1];
				//int n1 = faceValues[2];

				//int v2 = faceValues[3];
				//int t2 = faceValues[4];
				//int n2 = faceValues[5];

				//int v3 = faceValues[4];
				//int t3 = faceValues[5];
				//int n3 = faceValues[6];

				//faces.push_back(Face(v1, n1, t1, v2, n2, t2, v3, n3, t3));
			}//if face
		}

		calculateNorms();

	}

	void calculateNorms() {
		float ux, uy, uz;
		float vx, vy, vz;
		float nx, ny, nz;
		int p1, p2, p3;

		std::cout << "I calculated norms" << std::endl;

		for (int i = 0; i < faces.size(); i++) {
			p1 = faces[i].getP1();
			p2 = faces[i].getP2();
			p3 = faces[i].getP3();


			//p2 - p1
			ux = vertexes[p2].getX() - vertexes[p1].getX();
			uy = vertexes[p2].getY() - vertexes[p1].getY();
			uz = vertexes[p2].getZ() - vertexes[p1].getZ();

			//p3-p1
			vx = vertexes[p3].getX() - vertexes[p1].getX();
			vy = vertexes[p3].getY() - vertexes[p1].getY();
			vz = vertexes[p3].getZ() - vertexes[p1].getZ();

			//UxV Nx = UyVz - UzVy
			nx = (vy*uz) - (vz * uy);
			ny = (vz * ux) - (vx * uz);
			nz = (vx * uy) - (vy * ux);

			std::cout << "normal readfile " << nx << " " << ny << " " << nz << std::endl;
			faces[i].setNorm(nx, ny, nz);
			vertexes[p1].adjustNormal(nx, ny, nz);
			vertexes[p2].adjustNormal(nx, ny, nz);
			vertexes[p3].adjustNormal(nx, ny, nz);
		}
		for (int i = 0; i < vertexes.size(); i++) {

			vertexes[i].finishNormal();


		}

	}

	void getCoords(int state) {
		for (int i = 0; i < faces.size(); i++) {
			int p1, p2, p3;
			p1 = faces[i].getP1();
			p2 = faces[i].getP2();
			p3 = faces[i].getP3();

			//p1
			coords.push_back(vertexes[p1].getX());
			coords.push_back(vertexes[p1].getY());
			coords.push_back(vertexes[p1].getZ());

			coords.push_back(0.0);
			coords.push_back(1.0);
			coords.push_back(0.0);

			if (state == 1) {
				coords.push_back(faces[i].getNX());
				coords.push_back(faces[i].getNY());
				coords.push_back(faces[i].getNZ());
			}


			//p2
			coords.push_back(vertexes[p2].getX());
			coords.push_back(vertexes[p2].getY());
			coords.push_back(vertexes[p2].getZ());

			coords.push_back(0.0);
			coords.push_back(1.0);
			coords.push_back(0.0);

			if (state == 1) {
				coords.push_back(faces[i].getNX());
				coords.push_back(faces[i].getNY());
				coords.push_back(faces[i].getNZ());
			}

			//p3
			coords.push_back(vertexes[p3].getX());
			coords.push_back(vertexes[p3].getY());
			coords.push_back(vertexes[p3].getZ());

			coords.push_back(0.0);
			coords.push_back(1.0);
			coords.push_back(0.0);

			if (state == 1) {
				coords.push_back(faces[i].getNX());
				coords.push_back(faces[i].getNY());
				coords.push_back(faces[i].getNZ());
			}


		}



	}

	

	std::vector<float>& getAdd() {
		return coords;
	}


};

#endif