#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Tank : public Agent
{
public:

	void initialize();

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void draw(Level *level) override;

	float getEnergy() override;

	Vector2 getClosestMonsterPos();

private:

	void createBehaviourTree();

	const float maxEnergy = 200;
	float energy = maxEnergy;

	const float scale = 1.7f;
	float rotation = 0;
	Texture tankTex;

	// BehaviourTree stuff below
	BehaviourTree bT;
	BehaviourTree::Selector selector[2];
	Action moveTowardsHealer = Action("moveTowardsHealer", 100);
	Action moveTowardsPlayer = Action("moveTowardsPLayer", 100);


};