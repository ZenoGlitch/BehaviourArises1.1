#include "healer.h"

#include "level.h"


void Healer::initialize(Level* level)
{
    setType(type_healer);
    Vector2 pos = { (float)((GetScreenWidth() / 2) - 80.0f), (float)(GetScreenHeight() / 2) };
    setPosition(pos);
    setMoveSpeed(100.0f);
    //healerTex = LoadTexture("Assets/healer.png");
    //projectile.projectileTex = LoadTexture("Assets/projectile.png");
    
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
    if (energy <= 0.0f)
    {
        alive = false;
    }

    if (alive)
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

        if (playerHealth <= (playerMaxHealth / 4) * 3 || tankHealth <= (tankMaxHealth / 4) * 3)
        {
            if (playerHealthPercent < tankHealthPercent && level->tank.alive)
            {
                target = Agent::Target::Player;
                targetPos = level->player.getPosition();
            }
            else if (tankHealthPercent < playerHealthPercent && level->tank.alive)
            {
                target = Agent::Target::Tank;
                targetPos = level->tank.getPosition();
            }
            else if (!level->tank.alive)
            {
                target = Agent::Target::Player;
                targetPos = level->player.getPosition();
            }
            level->healer_checkAlliesHealth.condition = true;
        }
        else
        {
            level->healer_checkAlliesHealth.condition = false;
        }

        Vector2 pos = getPosition();
        Vector2 targetPos = getTargetPos();
        float distanceToTarget = Vector2Distance(pos, targetPos);
        if (distanceToTarget <= healRange)
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
        if (distanceToMonster <= attackRange)
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
    
        if (!projectile.active)
        {
            projectile.projectilePos = getPosition();
        }

        if (level->updateTick >= 2)
        {
            level->healer_selector[0].run(level, nullptr);
        }
    }
}

void Healer::draw(Level* level)
{
    if (alive)
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
        Rectangle rectSrc = { 0, 0, (float)level->healerTex.width, (float)level->healerTex.height};
        Rectangle rectDst = { pos.x, pos.y, (float)(level->healerTex.width * scale), (float)(level->healerTex.height * scale) };
        Vector2 origin = { (level->healerTex.width / 2) * scale, (level->healerTex.height / 2) * scale };

        DrawTexturePro(level->healerTex, rectSrc, rectDst, origin, angle, WHITE);

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

        if (projectile.active)
        {
            const float projectileScale = 2;
            Vector2 pPos = projectile.projectilePos;
            Rectangle projectileRectSrc = { 0, 0, (float)level->projectileTex.width, (float)level->projectileTex.height };
            Rectangle projectileRectDst = { pPos.x, pPos.y, (float)level->projectileTex.width * projectileScale, (float)level->projectileTex.height * projectileScale };
            Vector2 projectileOrigin = { (level->projectileTex.width / 2 * projectileScale), (level->projectileTex.height / 2 * projectileScale) };
            DrawTexturePro(level->projectileTex, projectileRectSrc, projectileRectDst, projectileOrigin, angle, WHITE);
        }
    }
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

void Healer::shoot(Level *level)
{
    projectile.coolDown -= GetFrameTime();

    if (projectile.coolDown <= 0.0f)
    {
        projectile.active = true;

        if (!projectile.positionsSet)
        {
            projectile.projectilePos = getPosition();
            projectile.targetPos = level->getClosestMonsterPos(this);
            projectile.positionsSet = true;
        }
    
        /*projectile.projectilePos = Vector2MoveTowards(projectile.projectilePos, monsterPos, attackRange);*/

    }

    if (projectile.positionsSet)
    {
        //float projectileMoveSpeed = 150;
        projectile.projectilePos = Vector2Lerp(projectile.projectilePos, projectile.targetPos, 0.01);


        //if (projectile.projectilePos.x < projectile.targetPos.x)
            //{
            //    projectile.projectilePos.x += GetFrameTime() * projectileMoveSpeed;
            //}
            //if (projectile.projectilePos.x > projectile.targetPos.x)
            //{
            //    projectile.projectilePos.x -= GetFrameTime() * projectileMoveSpeed;
            //}
            //if (projectile.projectilePos.y < projectile.targetPos.y)
            //{
            //    projectile.projectilePos.y += GetFrameTime() * projectileMoveSpeed;
            //}
            //if (projectile.projectilePos.y > projectile.targetPos.y)
            //{
            //    projectile.projectilePos.y -= GetFrameTime() * projectileMoveSpeed;
            //}

        Vector2 monsterPos = level->getClosestMonsterPos(this);

        if (projectile.projectilePos.x <= projectile.targetPos.x + 40 && projectile.projectilePos.x >= projectile.targetPos.x - 40
            && projectile.projectilePos.y <= projectile.targetPos.y + 40 && projectile.projectilePos.x >= projectile.targetPos.y - 40)
        {

            projectile.active = false;
            projectile.positionsSet = false;
            projectile.coolDown = 1;
        }

    }
}
