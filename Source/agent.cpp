#include "agent.h"

Vector2 Agent::getPosition()
{
	return this->position;
}

void Agent::setPosition(Vector2 p_position)
{
	this->position = p_position;
}

Vector2 Agent::getTargetPos()
{
	return targetPos;
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

void Agent::setType(Type p_type)
{
	type = p_type;
}

Agent::Type Agent::getType(Agent* agent)
{
	return agent->type;
}

void Agent::setMaxAttackCooldown(float p_maxCooldown)
{
	maxAttackCooldown = p_maxCooldown;
}

float Agent::getMaxAttackCooldown()
{
	return maxAttackCooldown;
}

void Agent::setAttackCooldown(float p_attackCooldown)
{
	attackCooldown = p_attackCooldown;
}

float Agent::getAttackCooldown()
{
	return attackCooldown;
}


void Agent::killAgent()
{
	this->dead = true;
}

bool Agent::isDead()
{
	return this->dead;
}