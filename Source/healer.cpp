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
    //DrawTextureEx(healerTex, pos, angle, scale, WHITE);
    Rectangle rectSrc = { 0, 0, (float)healerTex.width, (float)healerTex.height};
    Rectangle rectDst = { pos.x, pos.y, (float)(healerTex.width * scale), (float)(healerTex.height * scale) };
    Vector2 origin = { (healerTex.width / 2) * scale, (healerTex.height / 2) * scale };

    DrawTexturePro(healerTex, rectSrc, rectDst, origin, angle, WHITE);

    // Draw health bar
    const int borderSize = 4;
    const int halfBorderSize = borderSize / 2;
    const int healtBarHeight = 10;
    const int healthBarOffsetX = 20;
    const int healthBarPosX = (int)pos.x - (int)origin.x + healthBarOffsetX ;
    const int healthBarOffsetY = 20;
    const int healthBarPosY = (int)pos.y - (int)origin.y - healthBarOffsetY;
    DrawRectangle(healthBarPosX - halfBorderSize, healthBarPosY - halfBorderSize, (int)(maxEnergy / 2) + borderSize, healtBarHeight + borderSize, BLACK);
    DrawRectangle(healthBarPosX, healthBarPosY, (int)(energy / 2), healtBarHeight, RED);
}

float Healer::getEnergy()
{
    return energy;
}

float Healer::getMaxEnergy()
{
    return maxEnergy;
}

void Healer::createBehaviourTree()
{
    bT.setRootChild(&selector[0]);
    selector[0].addChild(&checkHealth);

}
