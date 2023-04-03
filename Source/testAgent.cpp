#include "testAgent.h"

#include "level.h"

#include <iostream>
#include <rlgl.h>

void TestAgent::initialize(Level *level)
{
	Vector2 pos = { (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2)};
	setPosition(pos);
	playerTex = LoadTexture("Assets/player.png");
	swordTex = LoadTexture("Assets/sword.png");
	level->pending_agents.push_back(this);
}

void TestAgent::sense(Level* level)
{
}

void TestAgent::decide()
{
}

void TestAgent::act(Level* level)
{
}

void TestAgent::draw(Level* level)
{
	Vector2 pos = getPosition();

	Vector2 mousePos = GetMousePosition();

	Vector2 sub = Vector2Subtract(mousePos, pos);
	Vector2 norm = Vector2Normalize(sub);
	Vector2 scaled = { norm.x * size, norm.y * size };
	//printf("mouse (%f, %f)  circle (%f, %f) \n", mousePos.x, mousePos.y, pos.x, pos.y);

	Vector2 lineEndPos = { (pos.x + scaled.x) , (pos.y + scaled.y) };

	//DrawCircle((int)pos.x, (int)pos.y, size, BLACK);
	//DrawLine((int)pos.x, (int)pos.y, (int)lineEndPos.x, (int)lineEndPos.y, RED);

	//Calulate angle
	Vector2 diff = Vector2Subtract(pos, mousePos);
	angle = -atan2f(diff.x, diff.y) * RAD2DEG;

	// Draw sword
	Rectangle swordRectSrc = { 0,0, (float)swordTex.width, (float)swordTex.height };
	Rectangle SwordRectDst = { lineEndPos.x, lineEndPos.y, (float)swordTex.width, (float)swordTex.height };
	Vector2 swordOrigin = { (float)(swordTex.width / 2), (float)swordTex.height };
	DrawTexturePro(swordTex, swordRectSrc, SwordRectDst, swordOrigin, angle, WHITE);
	swordTipPos = { lineEndPos.x + scaled.x * 2, lineEndPos.y + scaled.y * 2 };

	//DrawCircle(swordTipPos.x, swordTipPos.y, 10, BLUE);

	// Draw player
	const float scale = 1.5f;
	Rectangle playerRectSrc = { 0, 0, (float)playerTex.width, (float)playerTex.height };
	Rectangle playerRectDst = { pos.x, pos.y, (float)playerTex.width * scale, (float)playerTex.height * scale};
	const float pOriginX = (float)((playerTex.width / 2) * scale);
	const float pOriginY = (float)((playerTex.height / 2) * scale);
	Vector2 playerOrigin = { pOriginX, pOriginY };
	DrawTexturePro(playerTex, playerRectSrc, playerRectDst, playerOrigin, angle, WHITE );


	// Draw health bar
	const int borderSize = 4;
	const int halfBorderSize = borderSize / 2;
	const int healtBarHeight = 10;
	const int healthBarPosX = (int)pos.x - (int)pOriginX;
	const int healthBarOffsetY = 20;
	const int healthBarPosY = (int)pos.y - (int)pOriginY - healthBarOffsetY;
	DrawRectangle(healthBarPosX - halfBorderSize, healthBarPosY - halfBorderSize, (int)(maxEnergy / 2) + borderSize, healtBarHeight + borderSize, BLACK);
	DrawRectangle(healthBarPosX, healthBarPosY, (int)(energy / 2), healtBarHeight, RED);

}

float TestAgent::getEnergy()
{
	return energy;
}

float attacktimer = 5;
void TestAgent::attack()
{
	Vector2 pos = getPosition();


}
