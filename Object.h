#ifndef OBJECT_H
#define OBJECT_H

#include "config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Model.h"
#include "Lightbuffer.h"
#include "Camera.h"
#include "Colider.h"
#include "Animator.h"

using namespace glm;

class Object {
protected:
	Camera *camera;
	Model* MODEL;
	Shader* SHADER;
	Colider* COLIDER;

	mat4 model;
	mat4 view;
	mat4 projection;
	vec3 scale;
	float radian;
	vec3 rotateAxis;
	virtual void refreshMatrix()
	{
		SHADER->use();
		if (camera != nullptr)
		{
			view = camera->GetViewMatrix();
			projection = perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);//TODO
		}
		else
			cout << "ERROR::OBJECT camera is a null ptr" << endl;
		model = mat4(1.0f);
		model = rotate(model, radians(radian), rotateAxis);
		model = translate(model, position);
		model = glm::scale(model, scale);

		COLIDER->setPos(position);
	}
	Object()
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
	vec3 position;
	Object(const char* modelPath, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
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

	void setScale(vec3 scale)
	{
		this->scale = scale;
		COLIDER->setScale(scale);
		refreshMatrix();
	}
	void setPosition(vec3 position)
	{
		this->position = position;
		COLIDER->setPos(position);
		refreshMatrix();
	}
	void setRotate(float radians, vec3 axis)
	{
		this->radian = radians;
		this->rotateAxis = axis;
	}

	void setSkyBox(unsigned int tex) const
	{
		SHADER->setSkyBox(tex);
	}
	void setCamera(Camera& cam)
	{
		camera = &cam;
		refreshMatrix();
	}
	void setLight(Lightbuffer& buf)
	{
		SHADER->use();
		int dirLight_count = 0;
		int pointLight_count = 0;
		int spotLight_count = 0;
		for (int i = 0; i < buf.dirSize(); i++)
		{
			dirLight_count++;
			int unit = 15 - dirLight_count - pointLight_count - spotLight_count;
			SHADER->setDirLight(buf.getDirLight(i), dirLight_count, unit);
		}
		for (int i = 0; i < buf.pointSize(); i++)
		{
			pointLight_count++;
			int unit = 15 - dirLight_count - pointLight_count - spotLight_count;
			SHADER->setPointLight(buf.getPointLight(i), pointLight_count, unit);
		}
	}

	void setColider_positionOffset(const vec3 offset)
	{
		COLIDER->setPositionOffset(offset);
	}
	void setColider_scaleOffset(const vec3 offset)
	{
		COLIDER->setScaleOffset(offset);
	}
	bool Collision(Object& obj)
	{
		return COLIDER->ifCollision(obj.myColider());
	}
	bool Collision(const Colider& colider)
	{
		return COLIDER->ifCollision(colider);
	}
	bool Collision(const vec3 point)
	{
		return COLIDER->ifCollision(point);
	}
	
	mat4 getmodelmatrix()
	{
		return model;
	}
	Model& myModel()
	{
		return *MODEL;
	}
	//不建议使用此函数对碰撞体进行修改
	Colider& myColider()
	{
		return *COLIDER;
	}

	void Rend(bool ifdrawcolider = false)
	{
		refreshMatrix();
		SHADER->use();
		SHADER->setMat4("model", model);
		SHADER->setMat4("view", view);
		SHADER->setMat4("projection", projection);
		SHADER->setFloat("material.shininess", 32.0);
		SHADER->setVec3("viewPos", camera->Position);
		MODEL->Draw(*SHADER);
		if (ifdrawcolider)
			COLIDER->draw(view, projection);
	}
	void Rend(Shader& shader, bool ifdrawcolider = false)
	{
		refreshMatrix();
		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setFloat("material.shininess", 32.0);
		shader.setVec3("viewPos", camera->Position);
		MODEL->Draw(shader);
		if (ifdrawcolider)
			COLIDER->draw(view, projection);
	}

	// 为了规避在c++中不能使用instanceof的缺点而实现多态, 在GenerateShadow时使用的空方法
	virtual void updateAnimationMatrices(float dt){}
	// 为了规避在c++中不能使用instanceof的缺点而实现多态, 在GenerateShadow时使用的空方法
	virtual void updateAnimationMatrices(float dt, Shader& shader){}
	// 为了规避在c++中不能使用instanceof的缺点而实现多态, 在GenerateShadow时使用的空方法
	virtual void updateAnimationMatrices(float dt, Shader& shader1, Shader& shader2) {}
};

#endif // !OBJECT_H
