#include "monster.h"

#include "level.h"

void Monster::initialize(Level *level)
{
	setPosition(randSpawnPoint());
	monsterTex = LoadTexture("Assets/monster1.png");
	createBehaviourTree();
	level->pending_agents.push_back(this);
}

void Monster::sense(Level *level)
{
	Vector2 pos = getPosition();
	// what target is closest? Tank/Healer/Player?
	float distanceToPlayer = Vector2Distance(pos, level->tA.getPosition());
	float distanceToTank = Vector2Distance(pos, Vector2(10000, 10000)); // change this for tank pos
	float distanceToHealer = Vector2Distance(pos, Vector2(10000, 10000)); // change this for healer pos

	if (distanceToHealer < distanceToTank && distanceToHealer < distanceToPlayer)
	{
		healerIsClosest = true;
	}
	else healerIsClosest = false;
	if (distanceToPlayer < distanceToTank && distanceToPlayer < distanceToHealer)
	{
		playerIsClosest = true;
	}
	else playerIsClosest = false;
	if (distanceToTank < distanceToPlayer && distanceToTank < distanceToHealer)
	{
		tankIsClosest = true;
	}
	else tankIsClosest = false;

}

void Monster::decide()
{
	if (healerIsClosest)
	{
		target = Healer;
	}
	if (tankIsClosest)
	{
		target = Tank;
	}
	if (playerIsClosest)
	{
		target = Player;
	}

}

void Monster::act(Level *level)
{
	if (target == Player)
	{
		targetPos = level->tA.getPosition();
	}

	if (target == Tank)
	{
		targetPos = level->tank.getPosition();
	}
	//if (target == Tank)
	//{
	//	targetPos = level->tank.getPosition();
	//}
	//if (target == Healer)
	//{
	//	targetPos = level->healer.getPosition();
	//}

	moveTowards(targetPos);

	if (selector[0].run(level)) //THIS CRASHES WITH READ ACCESS VIOLATION WTH
	{
		printf("and returned successfully\n");
	}
	else
	{
		printf("something failed\n");
	}
}

void Monster::draw(Level *level)
{
	Vector2 pos = getPosition();
	//DrawCircle(pos.x, pos.y, size, BROWN);
	

	float angle = 0;
	//Calulate angle

	Vector2 diff = Vector2Subtract(pos, targetPos);
	angle = -atan2f(diff.x, diff.y) * RAD2DEG;


	float scale = 2;
	Rectangle rectSrc = { 0, 0, (float)monsterTex.width, (float)monsterTex.height };
	Rectangle rectDst = { pos.x, pos.y, monsterTex.width * scale, monsterTex.height * scale };
	Vector2 origin = { (monsterTex.width / 2) * scale, (monsterTex.height / 2) * scale };
	DrawTexturePro(monsterTex, rectSrc, rectDst, origin, angle + 30, WHITE);
}

float Monster::getEnergy()
{
	return energy;
}

Vector2 Monster::randSpawnPoint()
{
	Vector2 pos;
	int pickLeftOrRight = rand() % 3;
	int pickUpOrDown    = rand() % 3;

	if (pickLeftOrRight <= 1)
	{
		pos.x = size;
	}
	else pos.x = GetScreenWidth() - size;
	if (pickUpOrDown <= 1)
	{
		pos.y = size;
	}
	else pos.y = GetScreenHeight() - size;

	return pos;
}

void Monster::moveTowards(Vector2 target)
{
	Vector2 pos = getPosition();
	if (pos.x < target.x)
	{
		pos.x += moveSpeed * GetFrameTime();
	}
	else if (pos.x > target.x)
	{
		pos.x -= moveSpeed * GetFrameTime();
	}
	if (pos.y < target.y)
	{
		pos.y += moveSpeed * GetFrameTime();
	}
	else if (pos.y > target.y)
	{
		pos.y -= moveSpeed * GetFrameTime();
	}
	setPosition(pos);
}

void Monster::createBehaviourTree()
{

	bT.setRootChild(&selector[0]);
	selector[0].addChild(&testAction);
}

