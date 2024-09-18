#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <map>
#include <vector>
#include "Animation.h"
#include "Bone.h"

class Animator {
public:
	Animation* m_CurrentAnimation;

	Animator(Animation* current)
	{
		m_CurrentAnimation = current;
		m_CurrentTime = 0.0;
		m_Transforms.reserve(100);
		for (int i = 0; i < 100; i++)
			m_Transforms.push_back(glm::mat4(1.0f));

		Times = 0;
	}
	void PlayAnimation(Animation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;

		Times = 0;
	}
	void UpdateAnimation(float dt)
	{
		m_DeltaTime = dt;
		float m_PrevTime = m_CurrentTime;
		if (m_CurrentAnimation)
		{
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
		if (m_CurrentTime < m_PrevTime)
			Times++;
	}
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			m_Transforms[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}
	std::vector<glm::mat4> GetPoseTransforms()
	{
		return m_Transforms;
	}
	int GetTimes()
	{
		return Times;
	}
	float GetCurrentTime()
	{
		return m_CurrentTime;
	}
private:
	std::vector<glm::mat4> m_Transforms;
	float m_CurrentTime;
	float m_DeltaTime;
	int Times;
};
#endif // !ANIMATOR_H
