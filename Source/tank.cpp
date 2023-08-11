#include "tank.h"

#include "level.h"

void Tank::initialize(Level *level)
{
	setType(type_tank);
	Vector2 spawnPos = { (float)(GetScreenWidth() / 2) + 100, (float)(GetScreenHeight() / 2)};
	setPosition(spawnPos);
	setMoveSpeed(150.0f);
	level->pending_agents.push_back(this);
}

void Tank::sense(Level* level) // deprecated
{
}

void Tank::decide() // deprecated
{
}

void Tank::act(Level* level) // deprecated
{
}

void Tank::update(Level* level)
{
	Vector2 pos = getPosition();

	if (energy <= 0.0f)
	{
		alive = false;
	}

	if (alive)
	{
		if (energy <= maxEnergy / 4)
		{
			lowEnergy = true;

			level->tank_checkOwnHealth.setCondition(lowEnergy);
		}
		else
		{
			lowEnergy = false;
			level->tank_checkOwnHealth.setCondition(lowEnergy);
		}

		float healerHealth = level->healer.getEnergy();
		float healerMaxHealth = level->healer.getMaxEnergy();
		float healerHealthPercent = healerHealth / healerMaxHealth;

		float playerHealth = level->player.getEnergy();
		float playerMaxHealth = level->healer.getMaxEnergy();
		float playerHealthPercent = playerHealth / playerMaxHealth;

		Vector2 healerPos = level->healer.getPosition();
		Vector2 playerPos = level->player.getPosition();

		float distanceToHealer = Vector2Distance(pos, healerPos);
		float distanceToPlayer = Vector2Distance(pos, playerPos);

		if (distanceToHealer > 65.0f || distanceToPlayer > 65.0f)
		{
			closeToAlly = false;
		}
		else
		{
			closeToAlly = true;
		}

		if (!closeToAlly)
		{
			if (   healerHealth <= (healerMaxHealth / 4) * 3
				|| playerHealth <= (playerMaxHealth / 4) * 3 )
			{
				level->tank_checkAlliesHealth.condition = true;
			}
			else
			{
				level->tank_checkAlliesHealth.condition = false;
			}

			if (healerHealthPercent < playerHealthPercent && level->healer.alive)
			{
				target = Agent::Target::Healer;
				level->tank_moveToLowestHealthAlly.condition = true;
			}
			else
			{
				target = Agent::Target::Player;
				level->tank_moveToLowestHealthAlly.condition = false;
			}
		}

		Vector2 closestMonsterPos = level->getClosestMonsterPos(this);
		float distanceToClosestMonster = Vector2Distance(pos, closestMonsterPos);
		targetPos = closestMonsterPos;

		if (distanceToClosestMonster <= 100)
		{
			level->tank_notInAttackRange.condition = false;
			level->tank_inAttackRange.condition = true;
		}
		else
		{
			level->tank_inAttackRange.condition = false;
			level->tank_notInAttackRange.condition = true;
		}
		
		if (level->updateTick >= 2)
		{
			level->tank_selector[0].run(level, nullptr);
		}
	}
}

void Tank::draw(Level* level)
{
	if (alive)
	{
		Vector2 pos = getPosition();
		Vector2 origin = { (float)level->tankTex.width, (float)level->tankTex.height };
		Rectangle rectSrc = { 0.0f, 0.0f, (float)level->tankTex.width, (float)level->tankTex.height };
		Rectangle rectDst = { pos.x, pos.y, (float)(level->tankTex.width * scale), (float)(level->tankTex.height * scale) };

		DrawTexturePro(level->tankTex, rectSrc, rectDst, origin, angle, WHITE);

		// Draw health bar
		const int healthBarOffsetX = 10;
		const int healthBarPosX = (int)(pos.x - origin.x - healthBarOffsetX);
		const int healthBarOffsetY = 20;
		const int healthBarPosY = (int)(pos.y - origin.y - healthBarOffsetY);
		const int healthBarBorderPosX = healthBarPosX - level->healthBarHalfBorderSize;
		const int healthBarBorderPosY = healthBarPosY - level->healthBarHalfBorderSize;
		const int healthBarBorderWidth = (int)((maxEnergy / 2) + level->healthBarBorderSize);

		DrawRectangle(healthBarBorderPosX, healthBarBorderPosY, healthBarBorderWidth, level->healthBarBorderHeight, BLACK);
		DrawRectangle(healthBarPosX, healthBarPosY, (int)(energy / 2), level->healthBarHeight, RED);
	}
}

void Tank::heal(float p_health)
{
	float health = energy + p_health;
	if (health < maxEnergy)
	{
		energy = health;
	}
	else if (health >= maxEnergy)
	{
		energy = maxEnergy;
	}
}

void Tank::damage(float p_damage)
{
	energy = energy - p_damage;
}

float Tank::getEnergy()
{
	return energy;
}

float Tank::getMaxEnergy()
{
	return maxEnergy;
}

float Tank::getScale()
{
	return scale;
}

Vector2 Tank::getClosestMonsterPos(Level *level)
{
	float lastDistance = 10000000;
	float distance = 100000;
	Vector2 closestMonsterPos = { lastDistance, lastDistance };
	for (auto& monsters : level->monsterAgents)
	{
		Vector2 pos = getPosition();
		Vector2 monsterPos = monsters.getPosition();
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

