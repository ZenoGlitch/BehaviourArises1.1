#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Healer : public Agent
{
public:

	void initialize(Level *level);

	void sense(Level* level) override;
	void decide() override;
	void act(Level* level) override;

	void draw(Level* level) override;

	float getEnergy() override;

private:

	Texture healerTex;
	float angle;

	const float maxEnergy = 75;
	float energy = maxEnergy;


};