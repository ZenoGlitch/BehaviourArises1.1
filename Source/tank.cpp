#include "tank.h"

void Tank::initialize()
{
	tankTex = LoadTexture("Assets/tank.png");
	Vector2 spawnPos = { GetScreenWidth() / 2, GetScreenHeight() / 2};
	setPosition(spawnPos);
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

Vector2 Tank::getClosestMonsterPos()
{
	return Vector2(0,0);
}

