#pragma once

#include "agent.h"
#include "monster.h"
#include "behaviourTree.h"

class Healer : public Agent
{
public:

	void initialize(Level *level);

	void sense(Level* level) override;
	void decide() override;
	void act(Level* level) override;

	void update(Level* level);

	void draw(Level* level) override;


	float getEnergy() override;
	float getMaxEnergy();

	void heal(float p_health);
	void damage(float p_damage);

	void shoot(Level *level);
	

	bool drawHealCircle = false;

	struct Projectile
	{
		bool active = false;
		bool positionsSet = false;
		Vector2 projectilePos;
		Vector2 targetPos;
		float coolDown = 1;

	} projectile;

	bool alive = true;

private:
	

	const float maxEnergy = 75;
	float energy = maxEnergy;
	float attackRange = 550;
	float healRange = 350;


};