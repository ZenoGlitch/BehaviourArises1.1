#pragma once

#include "agent.h"

#include <raymath.h>

class TestAgent : public Agent
{
public:

	void initialize(Level* level);

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void update();

	void draw(Level *level) override;

	void heal(float p_health);
	void damage(float p_damage);

	float getEnergy() override;
	float getMaxEnergy();

	void attack();

	Vector2 swordTipPos;

	bool alive = true;

private:

	const float size = 25;
	const float maxEnergy = 100;
	float energy = maxEnergy;

	// Sword

	float angle;

};