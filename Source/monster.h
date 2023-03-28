#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Monster : public Agent
{
public:

	void initialize();

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void draw(Level *level) override;

	float getEnergy() override;

	Vector2 randSpawnPoint();
	void moveTowards(Vector2 target);

	void createBehaviourTree();


private:

	enum Target
	{
		Tank, Healer, Player, Count
	} target;

	Texture monsterTex;

	const float moveSpeed = 100;
	const float size = 20;
	float energy = 10;

	Vector2 targetPos;

	float distanceToTarget;

	bool playerIsClosest = false;
	bool tankIsClosest   = false;
	bool healerIsClosest = false;

	BehaviourTree bT;
	BehaviourTree::Selector selector[2];
	Action testAction = Action("testing1", 100);
};