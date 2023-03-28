#include "testAgent.h"

#include <iostream>
#include <rlgl.h>

void TestAgent::initialize()
{
	Vector2 pos = { GetScreenWidth() / 2, GetScreenHeight() / 2};
	setPosition(pos);
	playerTex = LoadTexture("Assets/player.png");
	swordTex = LoadTexture("Assets/sword.png");
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
	Rectangle swordRectSrc = { 0,0, swordTex.width, swordTex.height };
	Rectangle SwordRectDst = { lineEndPos.x, lineEndPos.y, swordTex.width, swordTex.height };
	Vector2 swordOrigin = { swordTex.width / 2, swordTex.height };
	DrawTexturePro(swordTex, swordRectSrc, SwordRectDst, swordOrigin, angle, WHITE);
	swordTipPos = { lineEndPos.x + scaled.x * 2, lineEndPos.y + scaled.y * 2 };
	//DrawCircle(swordTipPos.x, swordTipPos.y, 10, BLUE);

	// Draw player
	float scale = 1.5f;
	Rectangle playerRectSrc = { 0, 0, playerTex.width, playerTex.height };
	Rectangle playerRectDst = { pos.x, pos.y, playerTex.width * scale, playerTex.height * scale};
	Vector2 playerOrigin = { (playerTex.width / 2) * scale, (playerTex.height / 2) * scale };
	DrawTexturePro(playerTex, playerRectSrc, playerRectDst, playerOrigin, angle, WHITE );

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
