#include "level.h"

void Level::initialize()
{
	background = LoadTexture("Assets/background.png");
	tA.initialize();
	monster.initialize();
	tank.initialize();
}

void Level::input()
{
	const int moveSpeed = 200;
	Vector2 pos = tA.getPosition();
	if (IsKeyDown(KEY_W))
	{
		pos.y -= GetFrameTime() * moveSpeed;
		tA.setPosition(pos);
	}
	if (IsKeyDown(KEY_A))
	{
		pos.x -= GetFrameTime() * moveSpeed;
		tA.setPosition(pos);
	}
	if (IsKeyDown(KEY_S))
	{
		pos.y += GetFrameTime() * moveSpeed;
		tA.setPosition(pos);
	}
	if (IsKeyDown(KEY_D))
	{
		pos.x += GetFrameTime() * moveSpeed;
		tA.setPosition(pos);
	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		tA.attack();
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

	monster.sense(this);

	monster.decide();

	tA.act(this);
	monster.act(this);
	//std::cout << monster.getPosition().x << std::endl;

	//for(auto& agent : all_agents)
	//{
	//	// TODO: This piece of code needs to be changed to make sure that sense, decide, act, happen at different frequencies.
	//	agent->sense(this);
	//	agent->decide();
	//	agent->act(this);
	//}
}

void Level::draw()
{

	DrawTexture(background,0 ,0, WHITE);
	tank.draw(this);
	tA.draw(this);
	monster.draw(this);

	//for(auto& agent : all_agents)
	//{
	//	agent->draw(this);
	//}
}