#ifndef _CSCI441_UNIFORM_H_
#define _CSCI441_UNIFORM_H_

#include "matrix3.h"
#include "camera.h"
#include "model.h"
#include "uniform.h"

class Uniform {
public:
	static void set(GLuint shader, const std::string &name, const Matrix& m) {
		GLuint loc = location(shader, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, m.values);
	}

	static void set(GLuint shader, const std::string &name, const Matrix3& m) {
		GLuint loc = location(shader, name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, m.values);
	}

	static void set(GLuint shader, const std::string &name, const Vector& v) {
		GLuint loc = location(shader, name);
		glUniform3f(loc, v.values[0], v.values[1], v.values[1]);
	}

	static void set(GLuint shader, const std::string &name, const int v) {
		GLuint loc = location(shader, name);
		glUniform1i(loc, v);
	}

private:

	static GLuint location(GLuint shader, const std::string &name) {
		return glGetUniformLocation(shader, name.c_str());
	}
};

#endif