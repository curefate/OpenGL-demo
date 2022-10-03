#ifndef OBJECTSHADOWGENERATOR_H
#define OBJECTSHADOWGENERATOR_H

#include "config.h"

#include "Object.h"
#include <vector>
#include "Lightbuffer.h"

using namespace std;

class ObjectShadowGenerator {
private:
	vector<Object*> buffer;
	Shader* DSS;
	Shader* PSS;
public:
	ObjectShadowGenerator()
	{
		DSS = new Shader("Dir_Shadow_Shader.vs", "Dir_Shadow_Shader.fs");
		PSS = new Shader("Point_Shadow_Shader.vs", "Point_Shadow_Shader.fs", "Point_Shadow_Shader.gs");
	}
	void addObject(Object* obj)
	{
		buffer.push_back(obj);
	}
	int size()
	{
		return buffer.size();
	}
	Object& getObject(int i)
	{
		return *buffer[i];
	}

	void GenerateShadow(Lightbuffer& buf, float deltatime)
	{
		for (int i = 0; i < buf.dirSize(); i++)
		{
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, buf.getDirLight(i).depthFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			DSS->use();
			DSS->setMat4("lightSpaceMatrix", buf.getDirLight(i).lightSpaceMatrix);
			for (int j = 0; j < buffer.size(); j++)
			{
				DSS->setMat4("model", buffer[j]->getmodelmatrix());
				buffer[j]->updateAnimationMatrices(deltatime, *DSS); // animation
				buffer[j]->myModel().Draw(*DSS);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		}
		for (int i = 0; i < buf.pointSize(); i++)
		{
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, buf.getPointLight(i).depthFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			PSS->use();
			for (int j = 0; j < 6; j++)
				PSS->setMat4("shadowMatrices[" + to_string(j) + "]", buf.getPointLight(i).lightSpaceMatrix[j]);
			PSS->setVec3("lightPos", buf.getPointLight(i).position);
			PSS->setFloat("far_plane", buf.getPointLight(i).far_plane);
			for (int j = 0; j < buffer.size(); j++)
			{
				PSS->setMat4("model", buffer[j]->getmodelmatrix());
				buffer[j]->updateAnimationMatrices(deltatime, *PSS); // animation
				buffer[j]->myModel().Draw(*PSS);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/*
	void GenerateShadow(Lightbuffer& buf, float deltatime)
	{
		for (int obj = 0; obj < buffer.size(); obj++)
		{
			buffer[obj]->updateAnimationMatrices(deltatime, *DSS, *PSS);
			DSS->use();
			DSS->setMat4("model", buffer[obj]->getmodelmatrix());
			// Dir
			for (int lig = 0; lig < buf.dirSize(); lig++)
			{
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, buf.getDirLight(lig).depthFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				DSS->use();
				DSS->setMat4("lightSpaceMatrix", buf.getDirLight(lig).lightSpaceMatrix);
				buffer[obj]->myModel().Draw(*DSS);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			}
			// Point
			PSS->use();
			PSS->setMat4("model", buffer[obj]->getmodelmatrix());
			for (int lig = 0; lig < buf.pointSize(); lig++)
			{
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, buf.getPointLight(lig).depthFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				PSS->use();
				for (int j = 0; j < 6; j++)
					PSS->setMat4("shadowMatrices[" + to_string(j) + "]", buf.getPointLight(lig).lightSpaceMatrix[j]);
				PSS->setVec3("lightPos", buf.getPointLight(lig).position);
				PSS->setFloat("far_plane", buf.getPointLight(lig).far_plane);
				buffer[obj]->myModel().Draw(*PSS);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}*/
};

#endif // !OBJECTSHADOWGENERATOR_H
