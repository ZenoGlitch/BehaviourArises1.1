#include "testAgent.h"

#include "level.h"

#include <iostream>
#include <rlgl.h>

void TestAgent::initialize(Level *level)
{
	Vector2 pos = { (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2)};
	setPosition(pos);
	setMoveSpeed(200);
	level->pending_agents.push_back(this);
}

void TestAgent::sense(Level* level) // deprecated
{
}

void TestAgent::decide() // deprecated
{
}

void TestAgent::act(Level* level) // deprecated
{
}

void TestAgent::update()
{
	if (energy <= 0.0f)
	{
		alive = false;
	}
}

void TestAgent::draw(Level* level)
{
	Vector2 pos = getPosition();

	Vector2 mousePos = GetMousePosition();

	Vector2 sub = Vector2Subtract(mousePos, pos);
	Vector2 norm = Vector2Normalize(sub);
	Vector2 scaled = { norm.x * size, norm.y * size };

	Vector2 lineEndPos = { (pos.x + scaled.x) , (pos.y + scaled.y) };

	//Calulate angle
	Vector2 diff = Vector2Subtract(pos, mousePos);
	angle = -atan2f(diff.x, diff.y) * RAD2DEG;

	// Draw sword
	Rectangle swordRectSrc = { 0,0, (float)level->swordTex.width, (float)level->swordTex.height };
	Rectangle SwordRectDst = { lineEndPos.x, lineEndPos.y, (float)level->swordTex.width, (float)level->swordTex.height };
	Vector2 swordOrigin = { (float)(level->swordTex.width / 2), (float)level->swordTex.height };
	DrawTexturePro(level->swordTex, swordRectSrc, SwordRectDst, swordOrigin, angle, WHITE);
	swordTipPos = { lineEndPos.x + scaled.x * 2, lineEndPos.y + scaled.y * 2 };

	// Draw player
	const float scale = 1.5f;
	Rectangle playerRectSrc = { 0, 0, (float)level->playerTex.width, (float)level->playerTex.height };
	Rectangle playerRectDst = { pos.x, pos.y, (float)level->playerTex.width * scale, (float)level->playerTex.height * scale};
	const float pOriginX = (float)((level->playerTex.width / 2) * scale);
	const float pOriginY = (float)((level->playerTex.height / 2) * scale);
	Vector2 playerOrigin = { pOriginX, pOriginY };
	DrawTexturePro(level->playerTex, playerRectSrc, playerRectDst, playerOrigin, angle, WHITE );

	// Draw health bar
	const int healthBarPosX = (int)pos.x - (int)pOriginX;
	const int healthBarOffsetY = 20;
	const int healthBarPosY = (int)pos.y - (int)pOriginY - healthBarOffsetY;
	const int healthBarBorderPosX = healthBarPosX - level->healthBarHalfBorderSize;
	const int healthBarBorderPosY = healthBarPosY - level->healthBarHalfBorderSize;
	const int healthBarBorderWidth = (int)(maxEnergy / 2) + level->healthBarBorderSize;

	DrawRectangle(healthBarBorderPosX, healthBarBorderPosY, healthBarBorderWidth, level->healthBarBorderHeight, BLACK);
	DrawRectangle(healthBarPosX, healthBarPosY, (int)(energy / 2), level->healthBarHeight, RED);

}

void TestAgent::heal(float p_health)
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

void TestAgent::damage(float p_damage)
{
	energy = energy - p_damage;
}

float TestAgent::getEnergy()
{
	return energy;
}

float TestAgent::getMaxEnergy()
{
	return maxEnergy;
}

void TestAgent::attack()
{
	Vector2 pos = getPosition();
}
