#include "tank.h"

#include "level.h"

void Tank::initialize(Level *level)
{
	tankTex = LoadTexture("Assets/tank.png");
	Vector2 spawnPos = { (float)(GetScreenWidth() / 2) + 100, (float)(GetScreenHeight() / 2)};
	setPosition(spawnPos);
	level->pending_agents.push_back(this);
	createBehaviourTree();
}

void Tank::sense(Level* level)
{

}

void Tank::decide()
{

}

void Tank::act(Level* level)
{
	selector[0].run(level);
}

void Tank::draw(Level* level)
{
	Vector2 pos = getPosition();
	//DrawTexture(tankTex, pos.x, pos.y, WHITE);

	Vector2 origin = { tankTex.width, tankTex.height };
	//DrawTextureEx(tankTex, pos, rotation, scale, WHITE);
	Rectangle rectSrc = { 0, 0, tankTex.width, tankTex.height };
	Rectangle rectDst = { pos.x, pos.y, tankTex.width * scale, tankTex.height * scale };

	DrawTexturePro(tankTex, rectSrc, rectDst, origin, rotation, WHITE);

	// Draw health bar
	const float borderSize = 4;
	const float halfBorderSize = borderSize / 2;
	const float healtBarHeight = 10;
	const float healthBarOffsetX = 10;
	const float healthBarPosX = pos.x - origin.x - healthBarOffsetX;
	const float healthBarOffsetY = 20;
	const float healthBarPosY = pos.y - origin.y - healthBarOffsetY;
	DrawRectangle(healthBarPosX - halfBorderSize, healthBarPosY - halfBorderSize, (maxEnergy / 2) + borderSize, healtBarHeight + borderSize, BLACK);
	DrawRectangle(healthBarPosX, healthBarPosY, energy / 2, healtBarHeight, RED);
}

float Tank::getEnergy()
{
	return energy;
}

float Tank::getScale()
{
	return scale;
}

void Tank::createBehaviourTree()
{
	bT.setRootChild(&selector[0]);
	selector[0].addChild(&checkOwnHealth);
	selector[0].addChild(&moveTowardsPlayer);
}

Vector2 Tank::getClosestMonsterPos(Level *level)
{
	for (auto& monsters : level->monsterAgents)
	{
		
	}
		float lastDistance = 10000000;
		float distance = 100000;
		Vector2 closestMonsterPos = { lastDistance, lastDistance };
	for (int i = 0; i < level->monsterAgents.size(); ++i)
	{
		Vector2 pos = getPosition();
		Vector2 monsterPos = level->monsterAgents.at(i)->getPosition();
		if (distance < lastDistance)
		{
			lastDistance = distance;
			distance = Vector2Distance(pos, monsterPos);
			closestMonsterPos = monsterPos;

		}
	}

	return closestMonsterPos;
}

void Tank::setRotation(float p_rotation)
{
	rotation = p_rotation;
}

