#ifndef _CSCI441_MODEL_H_
#define _CSCI441_MODEL_H_

#include <shader.h>
#include "matrix.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Model {

public:
	GLuint vbo;
	GLuint vao;
	Shader shader;
	Matrix model;
	

	int size;
	GLuint program;



	template <typename Coords>
	Model(const Coords& coords,  const Shader& shader_in) : shader(shader_in) {
		size = coords.size() * sizeof(float);
		
		// copy vertex data
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, coords.data(), GL_STATIC_DRAW);
		//.data()
		// describe vertex layout
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		view.printValuse();
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
		//	(void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(2);

		program = shader.id();
	}

	void render(const Matrix& projection, const Matrix& view) {
		
		shader.use();

		float timeValue = glfwGetTime() * .5;

		

		int scaleLocation = glGetUniformLocation(program, "scale");
		glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, model.values);

		int viewLocation = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.values);

		int projectionLocation = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.values);
		

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, size);
	
	}
	void changeColor(float r , float g, float b) {
		int projectionLocation = glGetUniformLocation(program, "ourColor");
		glUniform3f(projectionLocation, r, g, b);
	}
	
};

#endif#pragma once
