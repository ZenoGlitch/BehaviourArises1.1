#include "monster.h"

#include "level.h"

void Monster::initialize()
{
	setType(type_monster);
	setPosition(randSpawnPoint());
	setMoveSpeed(100.0f);
	setMaxAttackCooldown(1.5f);
	setAttackCooldown(maxAttackCooldown);
}

void Monster::sense(Level *level) // deprecated
{
}

void Monster::decide() // deprecated
{
}

void Monster::act(Level *level) // deprecated
{
}

void Monster::update(Level *level)
{
	if (energy <= 0 && !isDead())
	{
		level->killCounter = level->killCounter + 1;
		
		if (level->killCounter % 3 == 0)
		{
			level->maxMonsterCount = level->maxMonsterCount + 2;
		}

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


	if (level->updateTick >= 2) // Run monster behaviour tree root
	{
		level->monster_selector.run(level, this);
	}
}

void Monster::draw(Level *level)
{
	Vector2 pos = getPosition();

	//Calulate angle

	Vector2 diff = Vector2Subtract(pos, targetPos);
	angle = -atan2f(diff.x, diff.y) * RAD2DEG;


	float scale = 2;
	Rectangle rectSrc = { 0, 0, (float)level->monsterTex.width, (float)level->monsterTex.height };
	Rectangle rectDst = { pos.x, pos.y, level->monsterTex.width * scale, level->monsterTex.height * scale };
	Vector2 origin = { (level->monsterTex.width / 2) * scale, (level->monsterTex.height / 2) * scale };
	DrawTexturePro(level->monsterTex, rectSrc, rectDst, origin, angle + 30, WHITE);


	// Draw health bar
	const int healthBarPosX = (int)(pos.x - origin.x / 2);
	const int healthBarOffsetY = 20;
	const int healthBarPosY = (int)(pos.y - origin.y - healthBarOffsetY);
	const int healthBarBorderPosX = healthBarPosX - level->healthBarHalfBorderSize;
	const int healthBarBorderPosY = healthBarPosY - level->healthBarHalfBorderSize;
	const int healthBarBorderWidth = (int)((maxEnergy / 2) + level->healthBarBorderSize);

	DrawRectangle(healthBarBorderPosX, healthBarBorderPosY, healthBarBorderWidth, level->healthBarBorderHeight, BLACK);
	DrawRectangle(healthBarPosX, healthBarPosY, (int)(energy / 2), level->healthBarHeight, RED);

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
}

void Monster::findClosestTarget(Level *level)
{
	Vector2 pos = getPosition();

	float distanceToPlayer = Vector2Distance(pos, level->player.getPosition());
	float distanceToTank   = Vector2Distance(pos, level->tank.getPosition()); 
	float distanceToHealer = Vector2Distance(pos, level->healer.getPosition()); 

	if (distanceToHealer < distanceToTank && distanceToHealer < distanceToPlayer && level->healer.alive)
	{
		distanceToTarget = distanceToHealer;
		target = Healer;
	}
	if (distanceToPlayer < distanceToTank && distanceToPlayer < distanceToHealer)
	{
		distanceToTarget = distanceToPlayer;
		target = Player;
	}
	if (distanceToTank < distanceToPlayer && distanceToTank < distanceToHealer && level->tank.alive)
	{
		distanceToTarget = distanceToTank;
		target = Tank;
	}
	if (!level->tank.alive && !level->healer.alive)
	{
		target = Player;
	}

	if (target == Player)
	{
		targetPos = level->player.getPosition();
	}

	if (target == Tank && level->tank.alive)
	{
		targetPos = level->tank.getPosition();
	}

	if (target == Healer && level->healer.alive)
	{
		targetPos = level->healer.getPosition();
	}
}


