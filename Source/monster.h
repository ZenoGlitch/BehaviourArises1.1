#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Monster : public Agent
{
public:
	
	void initialize();

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void update(Level* level);

	void draw(Level *level) override;

	float getEnergy() override;

	Vector2 randSpawnPoint();
	void moveTowards(Vector2 target);

	void damage(float damageAmount);

	void findClosestTarget(Level *level);

	//void setTargetPos(Vector2 p_targetPos);

	const float size = 20;

	bool recieveKnockback = false;
	bool isPositionsSet = false;

private:

	const float maxEnergy = 75;
	float energy = maxEnergy;

	float distanceToTarget = 10000000;

};