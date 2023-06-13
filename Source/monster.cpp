#include "monster.h"

#include "level.h"

//Monster::Monster()
//{
//	initialize(/*level*/);
//}

void Monster::initialize(/*Level *level*/)
{
	setType(type_monster);
	setPosition(randSpawnPoint());
	//monsterTex = LoadTexture("Assets/monster1.png");
	setMoveSpeed(75);
	setMaxAttackCooldown(1.5f);
	setAttackCooldown(maxAttackCooldown);
	//level->pending_agents.push_back(this);
}

void Monster::sense(Level *level)
{
	//Vector2 pos = getPosition();
	//// what target is closest? Tank/Healer/Player?
	//float distanceToPlayer = Vector2Distance(pos, level->player.getPosition());
	//float distanceToTank   = Vector2Distance(pos, level->tank.getPosition()); // change this for tank pos
	//float distanceToHealer = Vector2Distance(pos, level->healer.getPosition()); // change this for healer pos

	//if (distanceToHealer < distanceToTank && distanceToHealer < distanceToPlayer)
	//{
	//	healerIsClosest = true;
	//}
	//else healerIsClosest = false;
	//if (distanceToPlayer < distanceToTank && distanceToPlayer < distanceToHealer)
	//{
	//	playerIsClosest = true;
	//}
	//else playerIsClosest = false;
	//if (distanceToTank < distanceToPlayer && distanceToTank < distanceToHealer)
	//{
	//	tankIsClosest = true;
	//}
	//else tankIsClosest = false;

}

void Monster::decide()
{
	//if (healerIsClosest)
	//{
	//	target = Healer;
	//}
	//if (tankIsClosest)
	//{
	//	target = Tank;
	//}
	//if (playerIsClosest)
	//{
	//	target = Player;
	//}

}

void Monster::act(Level *level)
{
	//if (target == Player)
	//{
	//	targetPos = level->player.getPosition();
	//}

	//if (target == Tank)
	//{
	//	targetPos = level->tank.getPosition();
	//}
	////if (target == Tank)
	////{
	////	targetPos = level->tank.getPosition();
	////}
	////if (target == Healer)
	////{
	////	targetPos = level->healer.getPosition();
	////}

	//moveTowards(targetPos);

	////bT.run(level);

	////selector[0].run(level);

	////if (selector[0].run(level)) //THIS CRASHES WITH READ ACCESS VIOLATION WTH
	////{
	////	printf("and returned successfully\n");
	////}
	////else
	////{
	////	printf("something failed\n");
	////}
}

void Monster::update(Level *level)
{
	if (energy <= 0 && !isDead())
	{
		level->killCounter = level->killCounter + 1;
		level->maxMonsterCount = level->maxMonsterCount + 1;
		
		killAgent();
	}
	if (energy <= maxEnergy / 4)
	{
		level->monster_checkOwnHealth.setCondition(true);
	}
	else
	{
		level->monster_checkOwnHealth.setCondition(false);
	}

	if (recieveKnockback)
	{
		level->monster_attackedByTank.condition = true;
		recieveKnockback = false;
	}


	findClosestTarget(level);
	

	if (distanceToTarget <= 70)
	{
		level->monster_notInAttackRange.setCondition(false);
		level->monster_inAttackRange.setCondition(true);
		inAttackRange = true;
	}
	else
	{
		level->monster_inAttackRange.setCondition(false);
		level->monster_notInAttackRange.setCondition(true);
		inAttackRange = false;
	}



	level->monster_selector.run(level, this);
}

void Monster::draw(Level *level)
{
	Vector2 pos = getPosition();
	//DrawCircle(pos.x, pos.y, size, BROWN);

	//Calulate angle

	Vector2 diff = Vector2Subtract(pos, targetPos);
	angle = -atan2f(diff.x, diff.y) * RAD2DEG;


	float scale = 2;
	Rectangle rectSrc = { 0, 0, (float)level->monsterTex.width, (float)level->monsterTex.height };
	Rectangle rectDst = { pos.x, pos.y, level->monsterTex.width * scale, level->monsterTex.height * scale };
	Vector2 origin = { (level->monsterTex.width / 2) * scale, (level->monsterTex.height / 2) * scale };
	DrawTexturePro(level->monsterTex, rectSrc, rectDst, origin, angle + 30, WHITE);


	// Draw health bar
	const float borderSize = 4;
	const float halfBorderSize = borderSize / 2;
	const float healtBarHeight = 10;
	const float healthBarPosX = pos.x - origin.x / 2;
	const float healthBarOffsetY = 20;
	const float healthBarPosY = pos.y - origin.y - healthBarOffsetY;
	DrawRectangle(healthBarPosX - halfBorderSize, healthBarPosY - halfBorderSize, (maxEnergy / 2) + borderSize, healtBarHeight + borderSize, BLACK);
	DrawRectangle(healthBarPosX, healthBarPosY, energy / 2, healtBarHeight, RED);

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
		pos.x += getMoveSpeed() * GetFrameTime();
	}
	else if (pos.x > target.x)
	{
		pos.x -= getMoveSpeed() * GetFrameTime();
	}
	if (pos.y < target.y)
	{
		pos.y += getMoveSpeed() * GetFrameTime();
	}
	else if (pos.y > target.y)
	{
		pos.y -= getMoveSpeed() * GetFrameTime();
	}
	setPosition(pos);
}

void Monster::damage(float damageAmount)
{
	energy = energy - damageAmount;
	//printf("monster takes damage\n");
}

void Monster::findClosestTarget(Level *level)
{
	Vector2 pos = getPosition();
	// what target is closest? Tank/Healer/Player?
	float distanceToPlayer = Vector2Distance(pos, level->player.getPosition());
	float distanceToTank   = Vector2Distance(pos, level->tank.getPosition()); 
	float distanceToHealer = Vector2Distance(pos, level->healer.getPosition()); 

	if (distanceToHealer < distanceToTank && distanceToHealer < distanceToPlayer)
	{
		distanceToTarget = distanceToHealer;
		target = Healer;
	}
	if (distanceToPlayer < distanceToTank && distanceToPlayer < distanceToHealer)
	{
		distanceToTarget = distanceToPlayer;
		target = Player;
	}
	if (distanceToTank < distanceToPlayer && distanceToTank < distanceToHealer)
	{
		distanceToTarget = distanceToTank;
		target = Tank;
	}

	if (target == Player)
	{
		targetPos = level->player.getPosition();
	}

	if (target == Tank)
	{
		targetPos = level->tank.getPosition();
	}
}

void Monster::setTargetPos(Vector2 p_targetPos)
{
	targetPos = p_targetPos;
}


