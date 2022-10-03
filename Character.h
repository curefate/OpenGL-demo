#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "Object_Animeted.h"

class Character :public Object_Animeted
{
private:
	void refreshMatrix()
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

		front = normalize(front);
		vec3 Axis = vec3(0, 0, 1);
		vec3 v = cross(Axis, front);
		float myradian = acos(dot(Axis, front));
		if (v.y <= 0)
			myradian = 2 * 3.14159265 - myradian;
		model = rotate(model, myradian, vec3(0, 1, 0));

		COLIDER->setPos(position);
		colider_attack->setPos(position + vec3(front.x / 2, front.y / 2, front.z / 2));
		colider_extra->setPos(position);
	}
public:
	Character(const char* modelPath, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
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

		HP = 100;
		MP = 100;
		EP = 100;
		front = vec3(0, 0, 1);
		isDead = false;
		onceHited = false;
		isGurad = false;
		colider_attack = new Colider();
		colider_extra = new Colider();
	}
	float HP;
	float MP;
	float EP;
	vec3 front;
	bool isDead;
	bool onceHited;
	bool isGurad;
	Colider* colider_attack;
	Colider* colider_extra;
	int getDamege(int damege) {
		onceHited = true;
		return HP -= damege;
	}
};

#endif // !CHARACTER_H
