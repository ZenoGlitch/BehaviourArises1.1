#include "tank.h"

#include "level.h"

void Tank::initialize(Level *level)
{
	tankTex = LoadTexture("Assets/tank.png");
	Vector2 spawnPos = { (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2)};
	setPosition(spawnPos);
	level->pending_agents.push_back(this);
}

void Tank::sense(Level* level)
{

}

void Tank::decide()
{

}

void Tank::act(Level* level)
{

}

void Tank::draw(Level* level)
{
	Vector2 pos = getPosition();
	//DrawTexture(tankTex, pos.x, pos.y, WHITE);


	DrawTextureEx(tankTex, pos, rotation, scale, WHITE);
}

float Tank::getEnergy()
{
	return energy;
}

void Tank::createBehaviourTree()
{
	bT.setRootChild(&selector[0]);
	selector[0].addChild(&moveTowardsPlayer);
}

Vector2 Tank::getClosestMonsterPos(Level *level)
{
	for (auto& monsters : level->monsterAgents)
	{

	}

	return Vector2(0,0);
}

