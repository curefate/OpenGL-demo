#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "config.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static const float near_plane = 1.0f;
static const float far_plane = 7.5f;

class DirLight {
public:
	vec3 position;
	vec3 target;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	mat4 lightSpaceMatrix;
	unsigned int depthMap;
	unsigned int depthFBO;
public:
	DirLight(const vec3& position, const vec3& target, const vec3& ambient, const vec3& diffuse, const vec3& specular) {
		this->position = position;
		this->target = target;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		mat4 lightView = lookAt(this->position, this->target, vec3(0.0f, 1.0f, 0.0f));
		this->lightSpaceMatrix = lightProjection * lightView;

		glGenFramebuffers(1, &depthFBO);
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);//不适用任何颜色数据进行渲染
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "DirLight framebuffer not complete";
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};
#endif