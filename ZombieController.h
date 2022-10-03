#pragma once
#ifndef ZOMBIECONTROLLER_H
#define ZOMBIECONTROLLER_H

#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Character.h"

enum ZombieWeight {
	ZNONE = -1,
	ZIDLE,
	ZWALK,
	ZATTACK,
	ZHITED,
	ZDEADING,
	ZDEATH
};

class ZombieController {
public:
	Character* zombie;
	Animation* animation_death;
	Animation* animation_deading;
	Animation* animation_hited;
	Animation* animation_walk;
	Animation* animation_attack;
	Animation* animation_idle;
	Character* hero;

	ZombieWeight m_currentMovement;
	bool isAttacking;

	ZombieController(Character* zom, Character* her)
	{
		m_currentMovement = ZNONE;
		zombie = zom;
		hero = her;
		isAttacking = false;
	}

	/*void Running() {
		bool isMoving = false;
		// death
		if (zombie->isDead == true)
		{
			if (ZDEATH > m_currentMovement) {
				if (zombie->getCurrentAnimation() != animation_death)
					zombie->PlayAnimationByAnimator(animation_death);
				m_currentMovement = ZDEATH;
			}
		}
		else 
		{
			// deading
			if (zombie->HP <= 0.1 && !zombie->isDead) {
				if (ZDEADING > m_currentMovement) {
					oneTime = true;
					isMoving = true;
					zombie->isDead = true;
					if (zombie->getCurrentAnimation() != animation_deading)
						zombie->PlayAnimationByAnimator(animation_deading);
					m_currentMovement = ZDEADING;
				}
			}
			// hited
			if (zombie->onceHited == true) {
				if (ZHITED > m_currentMovement) {
					oneTime = true;
					isMoving = true;
					zombie->onceHited = false;
					if (zombie->getCurrentAnimation() != animation_hited)
						zombie->PlayAnimationByAnimator(animation_hited);
					m_currentMovement = ZHITED;
				}
			}
			// attack
			if (length(zombie->position - hero->position) < 0.3f) {
				if (ZATTACK > m_currentMovement) {
					oneTime = true;
					isMoving = true;
					if (zombie->getCurrentAnimation() != animation_attack)
						zombie->PlayAnimationByAnimator(animation_attack);
					m_currentMovement = ZATTACK;
					if (zombie->colider_attack->ifCollision(hero->myColider()))
						hero->getDamege(15);
				}
			}
			// walk
			if (zombie->colider_extra->ifCollision(hero->myColider())) {
				if (ZWALK > m_currentMovement) {
					isMoving = true;
					if (zombie->getCurrentAnimation() != animation_walk)
						zombie->PlayAnimationByAnimator(animation_walk);
					m_currentMovement = ZWALK;
					vec3 direction = normalize(hero->position - zombie->position);
					zombie->front = direction;
					zombie->position += zombie->front * ZOMBIE_WALK_SPEED;
				}
			}
			// idle
			if (!isMoving) {
				if (zombie->getCurrentAnimation() != animation_idle)
					zombie->PlayAnimationByAnimator(animation_idle);
				m_currentMovement = ZIDLE;
			}
		}
		if (oneTime) {
			if (zombie->getAnimationPlayTimes() > 1)
				oneTime = false;
		}
	}*/

	void Running()
	{
		if (zombie->HP < 0.1f && zombie->isDead) {
			zombie->PlayAnimationByAnimator(animation_death);
		}
		else if (zombie->HP < 0.1f && !zombie->isDead) {
			if (zombie->getCurrentAnimation() != animation_deading)
				zombie->PlayAnimationByAnimator(animation_deading);
			if (zombie->getAnimationPlayTimes() >= 1)
				zombie->isDead = true;
		}
		else if (zombie->onceHited) {
			if (zombie->getCurrentAnimation() != animation_hited)
				zombie->PlayAnimationByAnimator(animation_hited);
			if (zombie->getAnimationPlayTimes() >= 1)
				zombie->onceHited = false;
		}
		else if (length(zombie->position - hero->position) < 0.55f) {
			if (zombie->getCurrentAnimation() != animation_attack)
				zombie->PlayAnimationByAnimator(animation_attack);
			if (zombie->colider_attack->ifCollision(hero->myColider()) && zombie->getCurrentTime() > 0.8f && zombie->getCurrentTime() < 1.5f)
				if (hero->isGurad)
					hero->onceHited = true;
				else
					hero->getDamege(1);
		}
		else if (zombie->colider_extra->ifCollision(hero->myColider())) {
			if (zombie->getCurrentAnimation() != animation_walk)
				zombie->PlayAnimationByAnimator(animation_walk);
			zombie->front = -normalize(zombie->position - hero->position);
			zombie->position += zombie->front * ZOMBIE_WALK_SPEED;
		}
		else {
			if (zombie->getCurrentAnimation() != animation_idle)
				zombie->PlayAnimationByAnimator(animation_idle);
		}
	}
};

#endif // !ZOMBIECONTROLLER_H
