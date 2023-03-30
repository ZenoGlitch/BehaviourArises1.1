#include "healer.h"

void Healer::initialize(Level* level)
{
    Vector2 pos = { (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) };
    setPosition(pos);
    healerTex = LoadTexture("Assets/healer.png");
}

void Healer::sense(Level* level)
{
}

void Healer::decide()
{
}

void Healer::act(Level* level)
{
}

void Healer::draw(Level* level)
{
    Vector2 pos = getPosition();
    DrawTexture(healerTex, pos.x, pos.y, WHITE);
}

float Healer::getEnergy()
{
    return energy;
}
