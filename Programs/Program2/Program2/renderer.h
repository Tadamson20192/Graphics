#ifndef _CSCI441_RENDERER_H_
#define _CSCI441_RENDERER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uniform.h>
#include "matrix3.h"
#include "camera.h"
#include "model.h"

class Renderer {
	Matrix3 itModel_tmp;
public:

	void render(const Camera& camera, Model& m, const Vector& light) {
		itModel_tmp.inverse_transpose(m.model);

		m.shader.use();
		Uniform::set(m.shader.id(), "model", m.model);
		Uniform::set(m.shader.id(), "projection", camera.projection);
		Uniform::set(m.shader.id(), "camera", camera.look_at());
		Uniform::set(m.shader.id(), "eye", camera.eye);
		Uniform::set(m.shader.id(), "lightPos", light);
		Uniform::set(m.shader.id(), "itModel", itModel_tmp);

		// render the cube
		glBindVertexArray(m.vao);
		glDrawArrays(GL_TRIANGLES, 0, m.size);
	}
};

#endif