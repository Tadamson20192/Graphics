#ifndef _CSCI441_CAMERA_H_
#define _CSCI441_CAMERA_H_

#include "matrix.h"
#include "vector.h"

class Camera {
public:
	Matrix projection;
	Vector eye;
	Vector origin;
	Vector up;

	Camera() : eye(0, 0, 0), origin(0, 0, 0), up(0, 0, 0) {}

	Matrix look_at() const {
		Matrix mat;
		mat.look_at(eye, origin, up);
		return mat;
	}
};

#endif#pragma once
