#pragma once

#include "agent.h"

#include <raymath.h>

class TestAgent : public Agent
{
public:

	void initialize();

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void draw(Level *level) override;

	float getEnergy() override;

	void attack();
private:

	const float size = 25;
	const float maxEnergy = 100;
	float energy = maxEnergy;
	Texture playerTex;

	// Sword
	Texture swordTex;

	float angle;
	Vector2 swordTipPos;

};