#include "Initialization.h"

using namespace glm;

int main()
{
	if (Init() == -1)
		return -1;
	// Object
	//------------------------------------------------------------------
	camera = &camera2;
	ObjectShadowGenerator objbuffer;

	Skybox myskybox("skybox/");
	myskybox.setCamera(*camera);
	
	//Object lamp("resources/objects/lamp.fbx", "Blinn-Phone light.vs", "Blinn-Phone light.fs");
	//objbuffer.addObject(&lamp);
	//lamp.setCamera(*camera);
	//lamp.setRotate(90, vec3(1, 0, 0));
	//lamp.setScale(vec3(0.12));
	//lamp.setPosition(vec3(1,0,-2));

	Character Hero("resources/objects/Hero/Idle/Idle.dae", "Animation.vs", "Animation.fs");
	objbuffer.addObject(&Hero);
	Hero.setCamera(*camera);
	Hero.setPosition(vec3(0, -0.4, 0));
	Hero.setScale(vec3(0.5));
	Hero.setColider_positionOffset(vec3(0, 0.4, 0));
	Hero.setColider_scaleOffset(vec3(0.8, 1.6, 0.8));
	Hero.colider_attack->setPositionOffset(vec3(0, 0.6, 0));
	Hero.colider_attack->setScaleOffset(vec3(0.7, 0.4, 0.7));
	Hero.colider_extra->setPositionOffset(vec3(0, 0.4, 0));
	Animation hero_idle("resources/objects/Hero/Sword And Shield Idle/Sword And Shield Idle.dae", &Hero.myModel());
	Animation hero_walk("resources/objects/Hero/Sword And Shield Walk/Sword And Shield Walk.dae", &Hero.myModel());
	Animation hero_run("resources/objects/Hero/Sword And Shield Run/Sword And Shield Run.dae", &Hero.myModel());
	Animation hero_attack1("resources/objects/Hero/Sword And Shield Slash1/Sword And Shield Slash.dae", &Hero.myModel());
	Animation hero_attack2("resources/objects/Hero/Sword And Shield Slash2/Sword And Shield Slash.dae", &Hero.myModel());
	Animation hero_attack3("resources/objects/Hero/Sword And Shield Slash3/Sword And Shield Slash.dae", &Hero.myModel());
	Animation hero_guard("resources/objects/Hero/Sword And Shield Block Idle/Sword And Shield Block Idle.dae", &Hero.myModel());
	Animation hero_dance1("resources/objects/Hero/Twist Dance/Twist Dance.dae", &Hero.myModel());
	Animation hero_dance2("resources/objects/Hero/Northern Soul Spin Combo/Northern Soul Spin Combo.dae", &Hero.myModel());
	Animation hero_roll("resources/objects/Hero/Sprinting Forward Roll/Sprinting Forward Roll.dae", &Hero.myModel());
	Animation hero_hited("resources/objects/Hero/Sword And Shield Impact Front/Sword And Shield Impact.dae", &Hero.myModel());
	Animation hero_block("resources/objects/Hero/Sword And Shield Block/Sword And Shield Impact.dae", &Hero.myModel());
	Hero.InitAnimator(&hero_idle);
	HeroController heroController(Hero);
	heroController.setCamera(camera);
	heroController.addMovement(WALK_FRONT, &hero_walk);
	heroController.addMovement(WALK_BACK, &hero_walk);
	heroController.addMovement(WALK_LEFT, &hero_walk);
	heroController.addMovement(WALK_RIGHT, &hero_walk);
	heroController.addMovement(IDLE, &hero_idle);
	heroController.addMovement(RUN_FRONT, &hero_run);
	heroController.addMovement(RUN_BACK, &hero_run);
	heroController.addMovement(RUN_LEFT, &hero_run);
	heroController.addMovement(RUN_RIGHT, &hero_run);
	heroController.addMovement(ATTACK_1, &hero_attack1);
	heroController.addMovement(ATTACK_2, &hero_attack2);
	heroController.addMovement(ATTACK_3, &hero_attack3);
	heroController.addMovement(GUARD, &hero_guard);
	heroController.addMovement(DANCE_1, &hero_dance1);
	heroController.addMovement(DANCE_2, &hero_dance2);
	heroController.addMovement(ROLL_FRONT, &hero_roll);
	heroController.addMovement(ROLL_BACK, &hero_roll);
	heroController.addMovement(ROLL_LEFT, &hero_roll);
	heroController.addMovement(ROLL_RIGHT, &hero_roll);
	heroController.addMovement(HITED, &hero_hited);
	heroController.addMovement(HITED_BLOCK, &hero_block);

	Character Zombie("resources/objects/Zombie/Zombie Idle/Zombie Idle.dae", "AnimationZ.vs", "AnimationZ.fs");
	//objbuffer.addObject(&Zombie);
	//Zombie.setCamera(*camera);
	//Zombie.setPosition(vec3(-6, -0.4, 0));
	//Zombie.setScale(vec3(0.5f));
	//Zombie.setColider_positionOffset(vec3(0, 0.4, 0));
	//Zombie.setColider_scaleOffset(vec3(0.8, 1.6, 0.8));
	//Zombie.colider_attack->setPositionOffset(vec3(0, 0.6, 0));
	//Zombie.colider_attack->setScaleOffset(vec3(0.7, 0.4, 0.7));
	//Zombie.colider_extra->setPositionOffset(vec3(0, 0.4, 0));
	//Zombie.colider_extra->setScaleOffset(vec3(3, 1, 3));
	//heroController.setEnermy(&Zombie);
	//ZombieController zombieController(&Zombie, &Hero);
	//Animation zombie_idle("resources/objects/Zombie/Zombie Idle/Zombie Idle.dae", &Zombie.myModel());
	//Animation zombie_walk("resources/objects/Zombie/Walking/Walking.dae", &Zombie.myModel());
	//Animation zombie_attack("resources/objects/Zombie/Zombie Attack/Zombie Attack.dae", &Zombie.myModel());
	//Animation zombie_hited("resources/objects/Zombie/Zombie Reaction Hit/Zombie Reaction Hit.dae", &Zombie.myModel());
	//Animation zombie_deading("resources/objects/Zombie/Zombie Dying/Zombie Dying.dae", &Zombie.myModel());
	//Animation zombie_dead("resources/objects/Zombie/Zombie Dead/Zombie Dying.dae", &Zombie.myModel());
	//Zombie.InitAnimator(&zombie_idle);
	//zombieController.animation_attack = &zombie_attack;
	//zombieController.animation_deading = &zombie_deading;
	//zombieController.animation_death = &zombie_dead;
	//zombieController.animation_hited = &zombie_hited;
	//zombieController.animation_idle = &zombie_idle;
	//zombieController.animation_walk = &zombie_walk;

	Object city("resources/objects/base003.fbx", "Blinn-Phone light.vs", "Blinn-Phone light.fs");
	objbuffer.addObject(&city);
	city.setCamera(*camera);
	city.setRotate(270, vec3(0, 1, 0));
	city.setPosition(vec3(14, -0.5, 0));
	city.setScale(vec3(0.006));

	// Light
	//------------------------------------------------------------------
	DirLight DL(
		vec3(-2.0f, 4.0f, -1.0f),
		vec3(0),
		vec3(0.1f, 0.1f, 0.1f),
		vec3(0.5f, 0.5f, 0.5f),
		vec3(1.0f, 1.0f, 1.0f));
	PointLight PL(
		vec3(-1.0f, 1.0f, 0),
		vec3(0.1f),
		vec3(0.8f),
		vec3(1.0f),
		1.0, 0.007, 0.0002);
	Lightbuffer lightbuffer;
	lightbuffer.addLight(DL);
	lightbuffer.addLight(PL);
	
	// UI
	//------------------------------------------------------------------
	UI_bar life_barbase(vec2(-0.9,0.88), vec2(2.5,0.5), vec3(0.8, 0.05, 0.05), 0.5);
	UI_bar life_bar(vec2(-0.9, 0.88), vec2(2.5, 0.5), vec3(0.8, 0.05, 0.05), 1);
	UI_bar ep_barbase(vec2(-0.9, 0.78), vec2(2.5, 0.2), vec3(0.05, 0.9, 0.05), 0.5);
	UI_bar ep_bar(vec2(-0.9, 0.78), vec2(2.5, 0.2), vec3(0.05, 0.9, 0.05), 1);
	UI bardeco(vec2(-0.65, 0.8), vec2(4, 3));
	bardeco.setTexture("resources/picture/bardeco.png");

	// render loop
	//------------------------------------------------------------------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		// 设置参数 -----------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// 更新数据 -----------------------------------------------
		// 帧时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cout << deltaTime << endl;
		// 阴影
		objbuffer.GenerateShadow(lightbuffer, deltaTime);
		// 摄像机
		camera2.update(Hero.position + vec3(0, 0.6, 0));


		// skybox -----------------------------------------------
		myskybox.Draw();

		// object -----------------------------------------------
		Hero.setLight(lightbuffer);
		Hero.Rend();
		//Hero.colider_attack->draw(camera->GetViewMatrix(), perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
		//Hero.colider_extra->draw(camera->GetViewMatrix(), perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));

		//Zombie.setLight(lightbuffer);
		//Zombie.Rend();

		city.setLight(lightbuffer);
		city.Rend();
		
		//lamp.setLight(lightbuffer);
		//lamp.Rend();

		// UI -----------------------------------------------
		//bardeco.draw();
		//life_barbase.draw();
		//life_bar.setScale(vec2(Hero.HP * 2.5 / 100, 0.5));
		//life_bar.draw();
		//ep_barbase.draw();
		//ep_bar.setScale(vec2(Hero.EP * 2.5 / 100, 0.2));
		//ep_bar.draw();

		// response -----------------------------------------------
		glfwSwapBuffers(window);
		//processInput(window);
		heroController.Running(window);
		//zombieController.Running();
		glfwPollEvents();
	}
	glfwTerminate();
	
	return 0;
}