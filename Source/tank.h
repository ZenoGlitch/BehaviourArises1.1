#pragma once

#include "agent.h"
#include "behaviourTree.h"

class Tank : public Agent
{
public:

	void initialize(Level *level);

	void sense(Level *level) override;
	void decide() override;
	void act(Level *level) override;

	void draw(Level *level) override;

	float getEnergy() override;
	float getScale();

	Vector2 getClosestMonsterPos(Level *level);

	void setRotation(float p_rotation);

	const float moveSpeed = 100;
	const float maxEnergy = 200;

	enum State
	{
		runningAway,
		defendingPlayer, 
		defendingHealer,
		attacking,
		idle
	} state = idle;

private:

	void createBehaviourTree();

	float energy = maxEnergy;

	const float scale = 1.7f;
	float rotation = 0;
	Texture tankTex;



	// BehaviourTree stuff
	BehaviourTree bT;
	BehaviourTree::Selector selector[2];
	Action moveTowardsHealer = Action("moveTowardsHealer", 100);
	Action moveTowardsPlayer = Action("moveTowardsPlayer", 100);
	Action checkOwnHealth = Action("tankCheckOwnHealth", 100);


};