#include "tank.h"

#include "level.h"

void Tank::initialize(Level *level)
{
	tankTex = LoadTexture("Assets/tank.png");
	Vector2 spawnPos = { (float)(GetScreenWidth() / 2) + 100, (float)(GetScreenHeight() / 2)};
	setPosition(spawnPos);
	setMoveSpeed(100.0f);
	level->pending_agents.push_back(this);
	//createBehaviourTree();
}

void Tank::sense(Level* level)
{
	//if (energy <= maxEnergy / 4)
	//{
	//	lowEnergy = true;
	//}
	//else
	//{
	//	lowEnergy = false;
	//}
}

void Tank::decide()
{

}

void Tank::act(Level* level)
{
	//TankBT *BT = &BehaviourTree::tBT;
	//TankBT::Selector *selector111;
	//selector111 = TankBT::tBT.GetRoot();

	//selector[0].run(level);


}

void Tank::update(Level* level)
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


		if (healerHealthPercent < playerHealthPercent)
		{
			level->tank_moveToLowestHealthAlly.condition = true;
		}
		else
		{
			level->tank_moveToLowestHealthAlly.condition = false;
		}
	

	level->tank_selector[0].run(level);
}

void Tank::draw(Level* level)
{
	Vector2 pos = getPosition();
	//DrawTexture(tankTex, pos.x, pos.y, WHITE);

	Vector2 origin = { tankTex.width, tankTex.height };
	//DrawTextureEx(tankTex, pos, rotation, scale, WHITE);
	Rectangle rectSrc = { 0, 0, tankTex.width, tankTex.height };
	Rectangle rectDst = { pos.x, pos.y, tankTex.width * scale, tankTex.height * scale };

	DrawTexturePro(tankTex, rectSrc, rectDst, origin, angle, WHITE);

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
	//bT.setRootChild(&selector[0]);
	//selector[0].addChild(&checkOwnHealth);
	//selector[0].addChild(&moveTowardsPlayer);

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

