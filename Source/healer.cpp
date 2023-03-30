#include "healer.h"

#include "level.h"
void Healer::initialize(Level* level)
{
    Vector2 pos = { (float)((GetScreenWidth() / 2) - 80.0f), (float)(GetScreenHeight() / 2) };
    setPosition(pos);
    healerTex = LoadTexture("Assets/healer.png");
    level->pending_agents.push_back(this);
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
    //DrawTexture(healerTex, pos.x, pos.y, WHITE);
    const float scale = 1.5f;
    DrawTextureEx(healerTex, pos, angle, scale, WHITE);
}

float Healer::getEnergy()
{
    return energy;
}
