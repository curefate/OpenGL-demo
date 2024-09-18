#pragma once
#ifndef OBJECT_ANIMETED_H
#define OBJECT_ANIMETED_H

#include "Object.h"
#include "Animation.h"

class Object_Animeted : public Object
{
protected:
	Animator* ANIMATOR;
	Object_Animeted()
	{
		model = mat4(1.0f);
		view = mat4(1.0f);
		projection = mat4(1.0f);
		position = vec3(0);
		scale = vec3(1);
		radian = 0;
		rotateAxis = vec3(1);
	}
public:
	Object_Animeted(const char* modelPath, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		MODEL = new Model(modelPath);
		SHADER = new Shader(vertexPath, fragmentPath, geometryPath);
		COLIDER = new Colider();
		model = mat4(1.0f);
		view = mat4(1.0f);
		projection = mat4(1.0f);
		position = vec3(0);
		scale = vec3(1);
		radian = 0;
		rotateAxis = vec3(1);
	}

	void InitAnimator(Animation* DefaultAnimation)
	{
		ANIMATOR = new Animator(DefaultAnimation);
	}

	void updateAnimationMatrices(float dt)
	{
		ANIMATOR->UpdateAnimation(dt);
		SHADER->use();
		SHADER->setBool("ifAnimeted", true);
		auto transforms = ANIMATOR->GetPoseTransforms();
		for (int i = 0; i < transforms.size(); i++)
		{
			SHADER->setMat4("finalBoneMatrices[" + to_string(i) + "]", transforms[i]);
		}
	}
	void updateAnimationMatrices(float dt, Shader& shader)
	{
		ANIMATOR->UpdateAnimation(dt);
		auto transforms = ANIMATOR->GetPoseTransforms();
		SHADER->use();
		SHADER->setBool("ifAnimeted", true);
		for (int i = 0; i < transforms.size(); i++)
		{
			SHADER->setMat4("finalBoneMatrices[" + to_string(i) + "]", transforms[i]);
		}
		shader.use();
		shader.setBool("ifAnimeted", true);
		for (int i = 0; i < transforms.size(); i++)
		{
			shader.setMat4("finalBoneMatrices[" + to_string(i) + "]", transforms[i]);
		}
	}
	void updateAnimationMatrices(float dt, Shader& shader1, Shader& shader2)
	{
		ANIMATOR->UpdateAnimation(dt);
		auto transforms = ANIMATOR->GetPoseTransforms();
		SHADER->use();
		SHADER->setBool("ifAnimeted", true);
		for (int i = 0; i < transforms.size(); i++)
		{
			SHADER->setMat4("finalBoneMatrices[" + to_string(i) + "]", transforms[i]);
		}
		shader1.use();
		shader1.setBool("ifAnimeted", true);
		for (int i = 0; i < transforms.size(); i++)
		{
			shader1.setMat4("finalBoneMatrices[" + to_string(i) + "]", transforms[i]);
		}
		shader2.use();
		shader2.setBool("ifAnimeted", true);
		for (int i = 0; i < transforms.size(); i++)
		{
			shader2.setMat4("finalBoneMatrices[" + to_string(i) + "]", transforms[i]);
		}
	}

	int getAnimationPlayTimes()
	{
		return ANIMATOR->GetTimes();
	}
	Animation* getCurrentAnimation()
	{
		return ANIMATOR->m_CurrentAnimation;
	}
	float getCurrentTime()
	{
		return ANIMATOR->GetCurrentTime();
	}

	void PlayAnimationByAnimator(Animation* pAnimation)
	{
		ANIMATOR->PlayAnimation(pAnimation);
	}
};
#endif // !OBJECT_ANIMETED_H
