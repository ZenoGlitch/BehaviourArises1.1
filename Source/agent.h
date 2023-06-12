#pragma once

#include "raylib.h"

class Level;

class Agent
{
public:

	enum Type
	{
		type_tank, type_healer, type_monster, type_player
	} type;

	enum Target
	{
		Tank, Healer, Player, Monster, Count
	} target;


	int id = 0;

	virtual void sense(Level* level) = 0; // deprecated
	virtual void decide() = 0;			  // deprecated
	virtual void act(Level* level) = 0;   // deprecated

	virtual void draw(Level* level) = 0;

	Vector2 getPosition();
	void setPosition(Vector2 p_position);

	Vector2 getTargetPos();

	float getMoveSpeed();
	void setMoveSpeed(float p_moveSpeed);

	void setRotation(float p_angle);

	virtual float getEnergy() = 0;

	void setType(Type p_type);
	Type getType(Agent *agent);

	void setMaxAttackCooldown(float p_maxCooldown);
	float getMaxAttackCooldown();

	void setAttackCooldown(float p_cooldown);
	float getAttackCooldown();

	//void damage(float p_damage);
	void killAgent();
	bool isDead();


	virtual ~Agent() = default;

	bool inAttackRange = false;
	float maxAttackCooldown = -1;
	float attackCooldown = maxAttackCooldown;

private:

	Vector2 position = {};
	bool dead = false;

protected:
	float angle;
	float moveSpeed;
	Vector2 targetPos;
};