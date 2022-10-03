#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include "config.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm;
using namespace std;

class PointLight {
public:
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	vector<mat4> lightSpaceMatrix;
	unsigned int depthMap;
	unsigned int depthFBO;
	float far_plane;

	PointLight(const vec3& position, const vec3& ambient, const vec3& diffuse, const vec3& specular,
		const float constant, const float linear, const float quadratic)
	{
		this->position = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		
		float aspect = SHADOW_WIDTH / SHADOW_HEIGHT;
		float near = 0.1f;
		float far = 25.0f;
		far_plane = far;
		mat4 lightProjection = perspective(radians(90.0f), aspect, near, far);
		lightSpaceMatrix.push_back(lightProjection *
			lookAt(this->position, this->position + vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
		lightSpaceMatrix.push_back(lightProjection *
			lookAt(this->position, this->position + vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
		lightSpaceMatrix.push_back(lightProjection *
			lookAt(this->position, this->position + vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)));
		lightSpaceMatrix.push_back(lightProjection *
			lookAt(this->position, this->position + vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0)));
		lightSpaceMatrix.push_back(lightProjection *
			lookAt(this->position, this->position + vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0)));
		lightSpaceMatrix.push_back(lightProjection *
			lookAt(this->position, this->position + vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0)));

		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glGenFramebuffers(1, &depthFBO);
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
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

#endif // !POINTLIGHT_H