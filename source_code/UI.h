#ifndef UI_H
#define UI_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

using namespace glm;

class UI {
protected:
	float value;
	unsigned int VAO, VBO;
	unsigned int texture;
	vec2 position;
	vec2 scale;
	mat4 model;
	Shader* shader;
	unsigned int loadTexture(char const* path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
public:
	UI()
	{
		value = 0;
		shader = new Shader("UI.vs", "UI.fs");
		position = vec2(0);
		scale = vec2(1);
		model = mat4(1);
		float Vertices[] = {
		-0.1f,  0.1f,  0.0f, 1.0f,
		-0.1f, -0.1f,  0.0f, 0.0f,
		 0.1f, -0.1f,  1.0f, 0.0f,

		-0.1f,  0.1f,  0.0f, 1.0f,
		 0.1f, -0.1f,  1.0f, 0.0f,
		 0.1f,  0.1f,  1.0f, 1.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}
	UI(vec2 pos, vec2 sca)
	{
		shader = new Shader("UI.vs", "UI.fs");
		position = pos;
		scale = sca;
		model = mat4(1);
		model = translate(model, vec3(position, 0.0f));
		model = glm::scale(model, vec3(scale, 0.0f));
		value = 0;

		float Vertices[] = {
		-0.1f,  0.1f,  0.0f, 1.0f,
		-0.1f, -0.1f,  0.0f, 0.0f,
		 0.1f, -0.1f,  1.0f, 0.0f,

		- 0.1f,  0.1f,  0.0f, 1.0f,
		 0.1f, -0.1f,  1.0f, 0.0f,
		 0.1f,  0.1f,  1.0f, 1.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}
	void setPos(vec2 pos)
	{
		position = pos;
	}
	void setScale(vec2 sca)
	{
		scale = sca;
	}
	void setTexture(char const* path)
	{
		texture = loadTexture(path);
	}
	void setValue(float val)
	{
		value = val;
	}

	void draw()
	{
		model = mat4(1);
		model = translate(model, vec3(position, 0.0f));
		model = glm::scale(model, vec3(scale, 0.0f));
		shader->use();
		shader->setInt("texture1", 0);
		shader->setMat4("model", model);
		shader->setFloat("value", value);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
};

#endif // !UI_H
