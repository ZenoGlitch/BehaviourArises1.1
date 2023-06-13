#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Tank : public Agent
{
public:

	void initialize(Level *level);

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void update(Level* level);

	void draw(Level *level) override;

	void heal(float p_health);
	void damage(float p_damage);

	float getEnergy() override;
	float getMaxEnergy();
	float getScale();

	Vector2 getClosestMonsterPos(Level *level);

	void setRotation(float p_rotation);

	const float moveSpeed = 100;
	const float maxEnergy = 200;

	//enum State
	//{
	//	runningAway,
	//	defendingPlayer, 
	//	defendingHealer,
	//	attacking,
	//	idle
	//} state = idle;

	bool alive = true;

private:


	float energy = maxEnergy;
	bool lowEnergy = false;

	const float scale = 1.7f;
	float rotation = 0;

};