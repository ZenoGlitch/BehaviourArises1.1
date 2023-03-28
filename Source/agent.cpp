#include "agent.h"

void Agent::setPosition(Vector2 p_position)
{
	this->position = p_position;
}

Vector2 Agent::getPosition()
{
	return this->position;
}

void Agent::killAgent()
{
	this->dead = true;
}

bool Agent::isDead()
{
	return this->dead;
}