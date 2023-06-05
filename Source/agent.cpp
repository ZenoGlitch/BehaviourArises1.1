#include "agent.h"

Vector2 Agent::getPosition()
{
	return this->position;
}

void Agent::setPosition(Vector2 p_position)
{
	this->position = p_position;
}

float Agent::getMoveSpeed()
{
	return moveSpeed;
}

void Agent::setMoveSpeed(float p_moveSpeed)
{
	moveSpeed = p_moveSpeed;
}

void Agent::setRotation(float p_angle)
{
	angle = p_angle;
}

void Agent::killAgent()
{
	this->dead = true;
}

bool Agent::isDead()
{
	return this->dead;
}