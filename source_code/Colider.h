#ifndef COLIDER_H
#define COLIDER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

using namespace glm;

class Colider {
private:
	vec3 position;
	vec3 scale;
	vec3 posoffset;
	vec3 scaleoffset;
	vec3 far_point;
	vec3 near_point;
	void refreshVertex()
	{
		far_point = position + posoffset + vec3((scale * scaleoffset).x / 2, (scale * scaleoffset).y / 2, (scale * scaleoffset).z / 2);
		near_point = position + posoffset - vec3((scale * scaleoffset).x / 2, (scale * scaleoffset).y / 2, (scale * scaleoffset).z / 2);
	}
public:
	Colider()
	{
		position = vec3(0.0f);
		scale = vec3(1.0f);
		posoffset = vec3(0);
		scaleoffset = vec3(1);
		
		refreshVertex();
	}
	Colider(const vec3 pos, const vec3 sca)
	{
		position = pos;
		scale = sca;
		posoffset = vec3(0);
		scaleoffset = vec3(1);

		refreshVertex();
	}

	void setPos(const vec3 pos)
	{
		position = pos;
		refreshVertex();
	}
	void setScale(const vec3 sca)
	{
		scale = sca;
		refreshVertex();
	}
	void setPositionOffset(const vec3 posoffset)
	{
		this->posoffset = posoffset;
		refreshVertex();
	}
	void setScaleOffset(const vec3 scaleoffset)
	{
		this->scaleoffset = scaleoffset;
		refreshVertex();
	}

	bool ifCollision(const Colider& colider)
	{
		return
			((near_point.x >= colider.near_point.x && near_point.x <= colider.far_point.x) ||
				(colider.near_point.x >= near_point.x && colider.near_point.x <= far_point.x)) &&
			((near_point.y >= colider.near_point.y && near_point.y <= colider.far_point.y) ||
				(colider.near_point.y >= near_point.y && colider.near_point.y <= far_point.y)) &&
			((near_point.z >= colider.near_point.z && near_point.z <= colider.far_point.z) ||
				(colider.near_point.z >= near_point.z && colider.near_point.z <= far_point.z));
	}
	bool ifCollision(const vec3 point)
	{
		if (point.x < near_point.x)return false;
		if (point.y < near_point.y)return false;
		if (point.z < near_point.z)return false;
		if (point.x > far_point.x)return false;
		if (point.y > far_point.y)return false;
		if (point.z > far_point.z)return false;
		return true;
	}

	void draw(mat4 view, mat4 projection)
	{
		Shader shader("colider.vs", "colider.fs");
		mat4 model = mat4(1.0);
		model = translate(model, position + posoffset);
		model = glm::scale(model, scale * scaleoffset);

		float cubeVertices[] = {
			// Back face
		-0.5f, -0.5f, -0.5f, // Bottom-left
		 0.5f,  0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // bottom-right         
		 0.5f,  0.5f, -0.5f, // top-right
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f,  0.5f, -0.5f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-right
		 0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f,  0.5f, // top-left
		-0.5f, -0.5f,  0.5f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f,  0.5f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right         
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, // top-right
		 0.5f, -0.5f, -0.5f, // top-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-left
		-0.5f, -0.5f,  0.5f, // bottom-right
		-0.5f, -0.5f, -0.5f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f, // top-left
		 0.5f,  0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right     
		 0.5f,  0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f,  0.5f,  0.5f, // bottom-left   
		};
		unsigned int cubeVAO, cubeVBO;
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);
		
		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		glBindVertexArray(cubeVAO);
		glLineWidth(5);
		glDrawArrays(GL_LINE_LOOP, 0, 36);
		glLineWidth(1);
		glBindVertexArray(0);
	}
};

#endif // !COLIDER_H
