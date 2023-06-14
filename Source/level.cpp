#include "level.h"

void Level::initialize()
{
	loadTextures();
	player.initialize(this);
	tank.initialize(this);
	healer.initialize(this);
	initialize_behaviour_tree();
	
	spawnMonster(Monster());
	spawnMonster(Monster());
	for (auto& monsters : monsterAgents)
	{
		monsters.initialize(/*this*/);
		pending_agents.push_back(&monsters);
	}

}

void Level::initialize_behaviour_tree()
{	
	// Tank BT setup
	tank_selector[0].addChild(&tank_sequence[0]); // first branch
	tank_sequence[0].addChild(&tank_checkOwnHealth);
	tank_sequence[0].addChild(&tank_moveTowardsHealer);

	tank_selector[0].addChild(&tank_sequence[1]); // second branch
	tank_sequence[1].addChild(&tank_checkAlliesHealth);
	tank_sequence[1].addChild(&tank_moveToLowestHealthAlly);
	//tank_sequence[1].addChild(&tank_selector[1]);
	tank_selector[0].addChild(&tank_selector[1]); // third branch
	tank_selector[1].addChild(&tank_sequence[2]);
	tank_sequence[2].addChild(&tank_notInAttackRange);
	tank_sequence[2].addChild(&tank_moveTowardsMonster);
	tank_selector[1].addChild(&tank_sequence[3]);
	tank_sequence[3].addChild(&tank_inAttackRange);
	tank_sequence[3].addChild(&tank_attack);


	// Healer BT setup
	healer_selector[0].addChild(&healer_sequence[0]); // first branch
	healer_sequence[0].addChild(&healer_checkOwnHealth);
	healer_sequence[0].addChild(&healer_runAway);
	healer_selector[0].addChild(&healer_sequence[1]); // second branch
	healer_sequence[1].addChild(&healer_checkAlliesHealth);
	healer_sequence[1].addChild(&healer_selector[1]);
	healer_selector[1].addChild(&healer_sequence[2]);
	healer_sequence[2].addChild(&healer_notInHealingRange);
	healer_sequence[2].addChild(&healer_moveToLowestHealthAlly);
	healer_selector[1].addChild(&healer_sequence[3]);
	healer_sequence[3].addChild(&healer_inHealrange);
	healer_sequence[3].addChild(&healer_healTarget);
	healer_selector[0].addChild(&healer_selector[2]); // third branch
	healer_selector[2].addChild(&healer_sequence[4]);
	healer_sequence[4].addChild(&healer_notInAttackRange);
	healer_sequence[4].addChild(&healer_moveToMonster);
	healer_selector[2].addChild(&healer_sequence[5]);
	healer_sequence[5].addChild(&healer_inAttackRange);
	healer_sequence[5].addChild(&healer_attack);


	// Monster BT setup
	monster_selector.addChild(&monster_sequence[0]); // first branch
	monster_sequence[0].addChild(&monster_attackedByTank);
	monster_sequence[0].addChild(&monster_getKnockedBack);
	monster_selector.addChild(&monster_sequence[1]);
	monster_sequence[1].addChild(&monster_checkOwnHealth);
	monster_sequence[1].addChild(&monster_runAway);
	monster_selector.addChild(&monster_sequence[2]); // second branch
	monster_sequence[2].addChild(&monster_notInAttackRange);
	monster_sequence[2].addChild(&monster_moveToClosestTarget);
	monster_selector.addChild(&monster_sequence[3]); // third branch
	monster_sequence[3].addChild(&monster_inAttackRange);
	monster_sequence[3].addChild(&monster_attack);


}

void Level::loadTextures()
{
	background    = LoadTexture("Assets/background.png");
	playerTex     = LoadTexture("Assets/player.png");
	swordTex      = LoadTexture("Assets/sword.png");
	monsterTex    = LoadTexture("Assets/monster1.png");
	tankTex       = LoadTexture("Assets/tank.png");
	healerTex     = LoadTexture("Assets/healer.png");
	projectileTex = LoadTexture("Assets/projectile.png");
}

void Level::input()
{
	Vector2 pos = player.getPosition();
	float moveSpeed = player.getMoveSpeed();

	if (IsKeyDown(KEY_W) && pos.y > 0)
	{
		pos.y -= GetFrameTime() * moveSpeed;
		player.setPosition(pos);
	}
	if (IsKeyDown(KEY_A) && pos.x > 0)
	{
		pos.x -= GetFrameTime() * moveSpeed;
		player.setPosition(pos);
	}
	if (IsKeyDown(KEY_S) && pos.y < GetScreenHeight())
	{
		pos.y += GetFrameTime() * moveSpeed;
		player.setPosition(pos);
	}
	if (IsKeyDown(KEY_D) && pos.x < GetScreenWidth())
	{
		pos.x += GetFrameTime() * moveSpeed;
		player.setPosition(pos);
	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		player.attack();
	}
}

Agent* Level::get_agent(int id)
{
	auto agent_iterator = id_to_agent.find(id);
	if(agent_iterator != id_to_agent.end())
	{
		return agent_iterator->second;
	}

	return nullptr;
}

void Level::remove_dead_and_add_pending_agents()
{
	auto agent_iterator = all_agents.begin();
	while(agent_iterator != all_agents.end())
	{
		if((*agent_iterator)->isDead())
		{
			id_to_agent.erase((*agent_iterator)->id);
			agent_iterator = all_agents.erase(agent_iterator);
		} else 
		{
			agent_iterator++;
		}
	}

	// This must happen _after_ we remove agents from the vector 'all_agents'.
	// @AddMoreHere
	//silly_agents.remove_if([](SillyAgent& a){ return a.dead; });
	monsterAgents.remove_if([](Monster& a) {return a.isDead(); });


	// Add all pending agents
	for(Agent* agent: pending_agents)
	{
		last_id += 1;
		agent->id = last_id;

		all_agents.push_back(agent);
		id_to_agent.insert({agent->id, agent});
	}

	pending_agents.clear(); // Important that we clear this, otherwise we'll add the same agents every frame.
}

//Agent* Level::spawn_agent(SillyAgent agent)
//{
//	Agent* result = nullptr;
//
//	silly_agents.push_back(agent);
//	result = &silly_agents.back();
//	
//	pending_agents.push_back(result); // Enqueue it so that it can be added to the level at the beginning of the next frame
//
//	return result;
//}

Monster* Level::spawnMonster(Monster monster)
{
	Monster* result = nullptr;
	monsterAgents.push_back(monster);
	result = &monsterAgents.back();

	pending_agents.push_back(result);

	return result;
}

void Level::reset()
{
	// TODO: Implement this yourself, clear all lists and vectors, after that spawn agents

    // this is here just as an example.
    // You should also replace "SillyAgent", that is also just an example.

    //auto example = spawn_agent(SillyAgent());
    //example->position = {100,100};
}

void Level::update()
{

	remove_dead_and_add_pending_agents();
	updateTick ++;

	tank.update(this);
	healer.update(this);

	damageMonstersWithPlayersSword();

	for (auto& monsters : monsterAgents)
	{
		monsters.update(this);
	}

	if (monsterAgents.size() < maxMonsterCount)
	{
		for (int i = (int)monsterAgents.size(); i < maxMonsterCount; i++)
		{
			Monster* newMonster = spawnMonster(Monster());
			newMonster->initialize();
			pending_agents.push_back(newMonster);
		}
	}


	//for(auto& agent : all_agents)
	//{
	//	// TODO: This piece of code needs to be changed to make sure that sense, decide, act, happen at different frequencies.
	//	agent->sense(this);
	//	agent->decide();
	//	agent->act(this);
	//}

	if (updateTick >= 2.01)
	{
		updateTick = 0;
	}

}

void Level::draw()
{
	DrawTexture(background,0 ,0, WHITE);

	for(auto& agent : all_agents)
	{
		agent->draw(this);
	}

	DrawText(TextFormat("Kills: %i", killCounter), 30, GetScreenHeight() - 70, 50, BLACK);
}

void Level::damageMonstersWithPlayersSword()
{
	for (auto& monsters : monsterAgents)
	{
		if (!damageTaken)
		{
			if ( player.swordTipPos.x <= monsters.getPosition().x + monsters.size &&
				 player.swordTipPos.x >= monsters.getPosition().x - monsters.size &&
				 player.swordTipPos.y <= monsters.getPosition().y + monsters.size &&
			     player.swordTipPos.y >= monsters.getPosition().y - monsters.size )
			{
				monsters.damage(15);
				damageTaken = true;
			}
		}
	}

	if (damageTaken)
	{
		damageCooldown -= GetFrameTime();
	}
	if (damageCooldown <= 0)
	{
		damageCooldown = 2;
		damageTaken = false;
	}

	for (auto& monster : monsterAgents)
	{
		Vector2 healer_pPos = healer.projectile.projectilePos;
		Vector2 monsterPos = monster.getPosition();

		if (  healer_pPos.x > monsterPos.x - (monster.size + 5) && 
			  healer_pPos.x < monsterPos.x + (monster.size + 5) &&
			  healer_pPos.y > monsterPos.y - (monster.size + 5) &&
			  healer_pPos.y < monsterPos.y + (monster.size + 5) )
		{
			monster.damage(10);
			healer.projectile.active = false;
			healer.projectile.positionsSet = false;
			healer.projectile.coolDown = 1;
		}
	}
}

void Level::damageAgent(Agent& p_agent)
{
	
}

void Level::moveAgentTowardsOtherAgent(Agent& agentToMove, Vector2 targetAgentPos)
{
	Vector2 pos = agentToMove.getPosition();
	Vector2 targetPos = targetAgentPos;

	// Calulate angle
	Vector2 diff = Vector2Subtract(pos, targetPos);
	float angle = -atan2f(diff.x, diff.y) * RAD2DEG;
	// Rotate towards target
	if (pos.x >= targetPos.x + 10 || pos.x <= targetPos.x - 10)
	{
		if (pos.y >= targetPos.y + 10 || pos.y <= targetPos.y - 10)
		{
			
			agentToMove.setRotation(angle);
		}
	}

	// Move towards target
	if (targetPos.x > 0 && targetPos.x < GetScreenWidth())
	{
		if (pos.x < targetPos.x - 45)
		{
			pos.x += agentToMove.getMoveSpeed() * GetFrameTime();
		}
		if (pos.x > targetPos.x + 45)
		{
			pos.x -= agentToMove.getMoveSpeed() * GetFrameTime();
		}
	}
	if (targetPos.y > 0 && targetPos.y < GetScreenHeight())
	{
		if (pos.y < targetPos.y - 45)
		{
			pos.y += agentToMove.getMoveSpeed() * GetFrameTime();
		}
		if (pos.y > targetPos.y + 45)
		{
			pos.y -= agentToMove.getMoveSpeed() * GetFrameTime();
		}
	}

	agentToMove.setPosition(pos);
}

//Vector2 Level::getClosestMonsterPos(Agent *agent)
//{
//	float lastDistance = 10000000;
//	float distance = 100000;
//	Vector2 closestMonsterPos = { lastDistance, lastDistance };
//	for (auto& monsters : monsterAgents)
//	{
//		Vector2 pos = agent->getPosition();
//		Vector2 monsterPos = monsters.getPosition();
//		distance = Vector2Distance(pos, monsterPos);
//		if (distance < lastDistance)
//		{
//			lastDistance = distance;
//			closestMonsterPos = monsterPos;
//		}
//	}
//
//	return closestMonsterPos;
//}

Vector2 Level::getClosestMonsterPos(Agent* agent)
{
	float lastDistance = 10000000;
	float distance = 100000;
	Vector2 closestMonsterPos = { lastDistance, lastDistance };


	for (auto& monsters : monsterAgents)
	{
		Vector2 pos = agent->getPosition();
		Vector2 monsterPos = monsters.getPosition();
		distance = Vector2Distance(pos, monsterPos);
		if (distance < lastDistance)
		{
			lastDistance = distance;
			closestMonsterPos = monsterPos;
		}
	}

	return closestMonsterPos;
}