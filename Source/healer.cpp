#include "healer.h"

#include "level.h"


void Healer::initialize(Level* level)
{
    setType(type_healer);
    Vector2 pos = { (float)((GetScreenWidth() / 2) - 80.0f), (float)(GetScreenHeight() / 2) };
    setPosition(pos);
    setMoveSpeed(75.0f);
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

void Healer::update(Level* level)
{
    if (energy <= maxEnergy / 2)
    {
        level->healer_checkOwnHealth.condition = true;
    }
    else
    {
        level->healer_checkOwnHealth.condition = false;
    }

  
    float tankHealth = level->tank.getEnergy();
    float tankMaxHealth = level->tank.getMaxEnergy();
    float tankHealthPercent = tankHealth / tankMaxHealth;
    float playerHealth = level->player.getEnergy();
    float playerMaxHealth = level->player.getMaxEnergy();
    float playerHealthPercent = playerHealth / playerMaxHealth;

    if (playerHealth <= playerMaxHealth / 2 || tankHealth <= tankMaxHealth / 2)
    {
        if (playerHealthPercent < tankHealthPercent)
        {
            target = Agent::Target::Player;
            targetPos = level->player.getPosition();
        }
        else
        {
            target = Agent::Target::Tank;
            targetPos = level->tank.getPosition();
        }
        level->healer_checkAlliesHealth.condition = true;
    }
    else
    {
        level->healer_checkAlliesHealth.condition = false;
        //target = Agent::Target::Monster;
    }

    Vector2 pos = getPosition();
    Vector2 targetPos = getTargetPos();
    float distanceToTarget = Vector2Distance(pos, targetPos);
    if (distanceToTarget <= 350)
    {
        level->healer_notInHealingRange.condition = false;
        level->healer_inHealrange.condition = true;
        drawHealCircle = true;
    }
    else
    {
        level->healer_inHealrange.condition = false;
        level->healer_notInHealingRange.condition = true;
        drawHealCircle = false;
    }

    Vector2 monsterPos = level->getClosestMonsterPos(this);
    float distanceToMonster = Vector2Distance(pos, monsterPos);
    if (distanceToMonster <= 550)
    {
        inAttackRange = true;
        level->healer_notInAttackRange.condition = false;
        level->healer_inAttackRange.condition = true;
    }
    else
    {
        inAttackRange = false;
        level->healer_inAttackRange.condition = false;
        level->healer_notInAttackRange.condition = true;
    }


    

    

    

    level->healer_selector[0].run(level, nullptr);
}

void Healer::draw(Level* level)
{
    if (drawHealCircle)
    {
        Color healGreen = Fade(GREEN, 0.2);
        DrawCircle(targetPos.x, targetPos.y, 150, healGreen);
    }

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

void Healer::damage(float p_damage)
{
    energy = energy - p_damage;
}

void Healer::shoot()
{

}
