#pragma once
#ifndef HEROCONTROLLER_H
#define HEROCONTROLLER_H

#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Character.h"
#include "Camera_Follow.h"
#include <map>

using namespace std;
using namespace glm;

enum MoveMentWeight {
	NONE_W = -1,
	IDLE_W = 0,
	DANCE_W,
	WALK_W,
	RUN_W,
	INTERACT_W,
	GUARD_W,
	ATTACK_W,
	ROLL_W = ATTACK_W,
	HITED_W
};

enum MoveMentType {
	NONE = -1,
	// weight = 0
	IDLE, IDLE_OTHER,
	// weight = 1
	DANCE_1, DANCE_2, DANCE_3, DANCE_4,
	// weight = 2
	WALK_FRONT, WALK_BACK, WALK_LEFT, WALK_RIGHT,
	// weight = 3
	RUN_FRONT, RUN_BACK, RUN_LEFT, RUN_RIGHT,
	// weight = 4
	INTERACT_1, INTERACT_2, INTERACT_3, INTERACT_4,
	// weight = 5
	GUARD,
	// weight = 6
	ATTACK_1, ATTACK_2, ATTACK_3, ATTACK_4,
	ROLL_FRONT, ROLL_BACK, ROLL_LEFT, ROLL_RIGHT,
	// weight = 7
	HITED, HITED_BLOCK, HITED_BACK
};

class HeroController {
public:
	HeroController(Character& hero)
	{
		m_CurrentMoveMent = NONE;
		oneTime = false;
		canRun = true;
		isRolling = false;
		combo1to2 = 0;
		combo2to3 = 0;
		moveOffset_FB = 0;
		moveOffset_LR = 0;

		this->hero = &hero;

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::NONE, MoveMentWeight::NONE_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::IDLE, MoveMentWeight::IDLE_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::IDLE_OTHER, MoveMentWeight::IDLE_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::DANCE_1, MoveMentWeight::DANCE_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::DANCE_2, MoveMentWeight::DANCE_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::DANCE_3, MoveMentWeight::DANCE_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::DANCE_4, MoveMentWeight::DANCE_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::WALK_FRONT, MoveMentWeight::WALK_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::WALK_BACK, MoveMentWeight::WALK_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::WALK_LEFT, MoveMentWeight::WALK_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::WALK_RIGHT, MoveMentWeight::WALK_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::RUN_FRONT, MoveMentWeight::RUN_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::RUN_BACK, MoveMentWeight::RUN_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::RUN_LEFT, MoveMentWeight::RUN_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::RUN_RIGHT, MoveMentWeight::RUN_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::INTERACT_1, MoveMentWeight::INTERACT_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::INTERACT_2, MoveMentWeight::INTERACT_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::INTERACT_3, MoveMentWeight::INTERACT_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::INTERACT_4, MoveMentWeight::INTERACT_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::GUARD, MoveMentWeight::GUARD_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ATTACK_1, MoveMentWeight::ATTACK_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ATTACK_2, MoveMentWeight::ATTACK_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ATTACK_3, MoveMentWeight::ATTACK_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ATTACK_4, MoveMentWeight::ATTACK_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ROLL_FRONT, MoveMentWeight::ROLL_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ROLL_BACK, MoveMentWeight::ROLL_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ROLL_LEFT, MoveMentWeight::ROLL_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::ROLL_RIGHT, MoveMentWeight::ROLL_W));

		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::HITED, MoveMentWeight::HITED_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::HITED_BLOCK, MoveMentWeight::HITED_W));
		MoveMentWeightMap.insert(pair<MoveMentType, MoveMentWeight>(MoveMentType::HITED_BACK, MoveMentWeight::HITED_W));
	}
	void setCamera(Camera_Follow* camera)
	{
		this->camera = camera;
	}
	void setEnermy(Character* enermy)
	{
		this->enermy = enermy;
	}

	void addMovement(MoveMentType movementtype, Animation* animation)
	{
		MoveMentAnimationMap.insert(pair<MoveMentType, Animation*>(movementtype, animation));
	}

	void Running(GLFWwindow* window)
	{
		// value in logic decision
		bool isMoving = false;
		bool isxie = false;
		if (combo1to2 > 0)
			combo1to2 -= 0.1f;
		if (combo2to3 > 0)
			combo2to3 -= 0.1f;
		if (hero->EP < 100.0f)
			hero->EP += 0.1;
		if (hero->EP > 10.0f)
			canRun = true;

		// DANCE
		// ------------------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			if (MoveMentWeightMap.find(DANCE_1)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				oneTime = true;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(DANCE_1)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = DANCE_1;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			if (MoveMentWeightMap.find(DANCE_1)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				oneTime = true;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(DANCE_2)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = DANCE_2;
				}
			}
		}

		// WALK
		// ------------------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (MoveMentWeightMap.find(WALK_FRONT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(-normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_FRONT;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (MoveMentWeightMap.find(WALK_FRONT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_FRONT;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (MoveMentWeightMap.find(WALK_BACK)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = -normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_BACK;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (MoveMentWeightMap.find(WALK_BACK)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = -normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(-normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_BACK;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !isxie) {
			if (MoveMentWeightMap.find(WALK_FRONT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = direction;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_FRONT;
				}
				hero->position += hero->front * HERO_WALK_SPEED;

			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !isxie) {
			if (MoveMentWeightMap.find(WALK_LEFT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -normalize(cross(direction, vec3(0, 1, 0)));
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_LEFT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_LEFT;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !isxie) {
			if (MoveMentWeightMap.find(WALK_BACK)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -direction;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_BACK;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !isxie) {
			if (MoveMentWeightMap.find(WALK_RIGHT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(cross(direction, vec3(0, 1, 0)));
				Animation* newAnimationPointer = MoveMentAnimationMap.find(WALK_RIGHT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = WALK_RIGHT;
				}
				hero->position += hero->front * HERO_WALK_SPEED;
			}
		}

		// RUN
		// ------------------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun) {
			if (MoveMentWeightMap.find(RUN_FRONT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(-normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_FRONT;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun) {
			if (MoveMentWeightMap.find(RUN_FRONT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_FRONT;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun) {
			if (MoveMentWeightMap.find(RUN_BACK)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -normalize(normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_BACK;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun) {
			if (MoveMentWeightMap.find(RUN_BACK)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				isxie = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -normalize(-normalize(cross(direction, vec3(0, 1, 0))) + direction);
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_BACK;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun && !isxie) {
			if (MoveMentWeightMap.find(RUN_FRONT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = direction;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_FRONT;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun && !isxie) {
			if (MoveMentWeightMap.find(RUN_LEFT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -normalize(cross(direction, vec3(0, 1, 0)));
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_LEFT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_LEFT;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun && !isxie) {
			if (MoveMentWeightMap.find(RUN_BACK)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -direction;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_BACK;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && hero->EP > 0 && canRun && !isxie) {
			if (MoveMentWeightMap.find(RUN_RIGHT)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(cross(direction, vec3(0, 1, 0)));
				Animation* newAnimationPointer = MoveMentAnimationMap.find(RUN_RIGHT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = RUN_RIGHT;
				}
				hero->position += hero->front * HERO_RUN_SPEED;
				hero->EP -= 0.3f;
				if (hero->EP <= 0.1f)
					canRun = false;
			}
		}

		// INTERACT
		// ------------------------------------------------------------------------

		// GUARD
		// ------------------------------------------------------------------------
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS) {
			if (MoveMentWeightMap.find(GUARD)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				hero->isGurad = true;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(GUARD)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = GUARD;
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
			hero->isGurad = false;
		}

		// ATACK 1-3  攻击动画在判定权值时，权值相等不能替换
		// ------------------------------------------------------------------------
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && hero->EP > 10.0f && combo1to2 < 0.11f && combo2to3 < 0.11f) {
			if (MoveMentWeightMap.find(ATTACK_1)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				combo1to2 = 15.0f;
				hero->EP -= 10.0f;
				if (enermy != nullptr && hero->colider_attack->ifCollision(enermy->myColider()))
				{
					enermy->getDamege(20);
				}
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ATTACK_1)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ATTACK_1;
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && hero->EP > 15.0f && combo1to2 > 5.0f && combo2to3 < 0.11f) {
			if (MoveMentWeightMap.find(ATTACK_2)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				combo2to3 = 15.0f;
				hero->EP -= 15.0f;
				if (enermy != nullptr && hero->colider_attack->ifCollision(enermy->myColider()))
				{
					enermy->getDamege(20);
				}
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ATTACK_2)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ATTACK_2;
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && hero->EP > 20.0f && combo2to3 > 4.0f) {
			if (MoveMentWeightMap.find(ATTACK_3)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				hero->EP -= 20.0f;
				if (enermy != nullptr && hero->colider_attack->ifCollision(enermy->myColider()))
				{
					enermy->getDamege(20);
				}
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ATTACK_3)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ATTACK_3;
				}
				moveOffset_FB = 0.025f;
			}
		}

		// ROLL  翻滚动画在判定权值时，权值相等不能替换
		// ------------------------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && hero->EP > 10.0) {
			if (MoveMentWeightMap.find(ROLL_FRONT)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				isRolling = true;
				hero->EP -= 10.0f;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = direction;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ROLL_FRONT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ROLL_FRONT;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && hero->EP > 10.0) {
			if (MoveMentWeightMap.find(ROLL_BACK)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				isRolling = true;
				hero->EP -= 10.0f;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -direction;
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ROLL_BACK)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ROLL_BACK;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && hero->EP > 10.0) {
			if (MoveMentWeightMap.find(ROLL_LEFT)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				isRolling = true;
				hero->EP -= 10.0f;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = -normalize(cross(direction, vec3(0, 1, 0)));
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ROLL_LEFT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ROLL_LEFT;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && hero->EP > 10.0) {
			if (MoveMentWeightMap.find(ROLL_RIGHT)->second > MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				isRolling = true;
				hero->EP -= 10.0f;
				vec3 direction = normalize(vec3(camera->direction.x, 0, camera->direction.z));
				hero->front = normalize(cross(direction, vec3(0, 1, 0)));
				Animation* newAnimationPointer = MoveMentAnimationMap.find(ROLL_RIGHT)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = ROLL_RIGHT;
				}
			}
		}
		if (isRolling) {
			if (oneTime) {
				hero->position += hero->front * HERO_ROLL_SPEED;
			}
		}

		// HITED
		// ------------------------------------------------------------------------
		if (hero->onceHited == true)
		{
			if (MoveMentWeightMap.find(HITED)->second >= MoveMentWeightMap.find(m_CurrentMoveMent)->second) {
				isMoving = true;
				oneTime = true;
				hero->onceHited = false;
				if (hero->isGurad == true)
				{
					Animation* newAnimationPointer = MoveMentAnimationMap.find(HITED_BLOCK)->second;
					if (hero->getCurrentAnimation() != newAnimationPointer) {
						hero->PlayAnimationByAnimator(newAnimationPointer);
						m_CurrentMoveMent = HITED_BLOCK;
					}
				}
				else
				{
					Animation* newAnimationPointer = MoveMentAnimationMap.find(HITED)->second;
					if (hero->getCurrentAnimation() != newAnimationPointer) {
						hero->PlayAnimationByAnimator(newAnimationPointer);
						m_CurrentMoveMent = ROLL_FRONT;
					}
				}
			}
		}

		// BACK TO IDLE
		// ------------------------------------------------------------------------
		if (!isMoving)
		{
			if (oneTime) {
				if (hero->getAnimationPlayTimes() >= 1) {
					oneTime = false;
					isRolling = false;
					Animation* newAnimationPointer = MoveMentAnimationMap.find(IDLE)->second;
					if (hero->getCurrentAnimation() != newAnimationPointer) {
						hero->PlayAnimationByAnimator(newAnimationPointer);
						m_CurrentMoveMent = IDLE;
					}
					if (moveOffset_FB >= 0.001f || moveOffset_FB <= -0.001f) {
						hero->position += normalize(hero->front) * moveOffset_FB;
						moveOffset_FB = 0;
					}
					if (moveOffset_LR >= 0.001f || moveOffset_LR <= -0.001f) {
						hero->position += normalize(cross(hero->front, vec3(0, 1, 0))) * moveOffset_LR;
						moveOffset_LR = 0;
					}
				}
			}
			else {
				Animation* newAnimationPointer = MoveMentAnimationMap.find(IDLE)->second;
				if (hero->getCurrentAnimation() != newAnimationPointer) {
					hero->PlayAnimationByAnimator(newAnimationPointer);
					m_CurrentMoveMent = IDLE;
				}
			}
		}
	}
private:
	Character* hero;
	Character* enermy;
	Camera_Follow* camera;
	map<MoveMentType, Animation*> MoveMentAnimationMap;
	map<MoveMentType, MoveMentWeight> MoveMentWeightMap;

	MoveMentType m_CurrentMoveMent;
	bool oneTime;
	bool canRun;
	bool isRolling;
	float combo1to2;
	float combo2to3;
	float moveOffset_FB; // front and back
	float moveOffset_LR; // left and right
};

#endif // !HEROCONTROLLER_H
