#pragma once
#ifndef UI_BAR_H
#define UI_BAR_H
#include "UI.h"

class UI_bar : public UI {
public:
	UI_bar()
	{
		value = 0;
		shader = new Shader("UI.vs", "UI.fs");
		position = vec2(0);
		scale = vec2(1);
		model = mat4(1);
		float Vertices[] = {
		-0.1f,  0.1f,  0.0f, 0.0f, 0.0f,
		-0.1f, -0.1f,  0.0f, 0.0f, 0.0f,
		 0.1f, -0.1f,  0.0f, 0.0f, 0.0f,

		-0.1f,  0.1f,  0.0f, 0.0f, 0.0f,
		 0.1f, -0.1f,  0.0f, 0.0f, 0.0f,
		 0.1f,  0.1f,  0.0f, 0.0f, 0.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}
	UI_bar(vec2 pos, vec2 sca, vec3 color, float val = 1)
	{
		value = val;
		shader = new Shader("UIbar.vs", "UIbar.fs");
		position = pos;
		scale = sca;
		model = mat4(1);
		model = translate(model, vec3(position, 0.0f));
		model = glm::scale(model, vec3(scale, 0.0f));

		float Vertices[] = {
		 0.0f,  0.0f,  color.x, color.y, color.z,
		 0.0f, -0.2f,  color.x, color.y, color.z,
		 0.2f, -0.2f,  color.x, color.y, color.z,

		 0.2f,  -0.2f,  color.x, color.y, color.z,
		 0.2f,  0.0f,  color.x, color.y, color.z,
		 0.0f,  0.0f,  color.x, color.y, color.z
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}
};

#endif // !UI_BAR_H
