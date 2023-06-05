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

	Vector2 getPosition();
	void setPosition(Vector2 p_position);

	float getMoveSpeed();
	void setMoveSpeed(float p_moveSpeed);

	void setRotation(float p_angle);

	virtual float getEnergy() = 0;

	void killAgent();
	bool isDead();


	virtual ~Agent() = default;

private:

	Vector2 position = {};
	bool dead = false;

protected:
	float angle;
	float moveSpeed;
};