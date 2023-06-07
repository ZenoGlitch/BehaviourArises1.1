#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Monster : public Agent
{
public:
	//Monster() = default;

	void initialize(Level *level);

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void update(Level* level);

	void draw(Level *level) override;

	float getEnergy() override;

	Vector2 randSpawnPoint();
	void moveTowards(Vector2 target);

	void damage(float damageAmount);

	void createBehaviourTree();

	void findClosestTarget(Level *level);
	//Vector2 getTargetPos();
	void setTargetPos(Vector2 p_targetPos);

	const float size = 20;

	//enum Target
	//{
	//	Tank, Healer, Player, Count
	//} target;

	//bool inAttackRange = false;

private:


	Texture monsterTex;

	//const float moveSpeed = 75;

	const float maxEnergy = 75;
	float energy = maxEnergy;

	//Vector2 targetPos;

	float distanceToTarget;

	//bool playerIsClosest = false;
	//bool tankIsClosest   = false;
	//bool healerIsClosest = false;


	//BehaviourTree bT;
	//BehaviourTree::Selector selector[2];
	//Action testAction = Action("testing1", 100);
};