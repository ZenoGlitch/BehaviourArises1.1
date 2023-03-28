#pragma once

#include "raylib.h"

class Level;

class Agent
{
public:

	int id = 0;

	virtual void sense(Level* level) = 0;
	virtual void decide() = 0;
	virtual void act(Level* level) = 0;

	virtual void draw(Level* level) = 0;

	void setPosition(Vector2 p_position);

	Vector2 getPosition();

	virtual float getEnergy() = 0;

	void killAgent();
	bool isDead();


	virtual ~Agent() = default;

private:

	Vector2 position = {};
	bool dead = false;


};