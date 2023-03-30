#pragma once
#include "raylib.h"

#include "agent.h"
#include "testAgent.h"
#include "monster.h"
#include "tank.h"
#include "healer.h"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <random>
#include <raymath.h>

class Level;
	
class Level 
{
public:
	TestAgent tA;
	Monster monster;
	Tank tank;
	Healer healer;

	std::vector<Monster*> monsterAgents;

	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame

private:
	int last_id = 0;

	//NOTE(Filippo): Using a list here is not the best idea, ideally you should store agents in some other data structure that keeps them close to each other while being pointer-stable.
	//std::list<SillyAgent> silly_agents;
	// @AddMoreHere

	std::unordered_map<int, Agent*> id_to_agent;
	std::vector<Agent*> all_agents;

	Texture background;

public:
	Agent* get_agent(int id);

	//Agent* spawn_agent(SillyAgent agent);
	Monster* spawnMonster(Monster &monster);
	// @AddMoreHere

	void initialize();

	void input();

	void reset();
	void update();
	void draw();

private:
	void remove_dead_and_add_pending_agents();
	// Remember, if you add more lists (see @AddMoreHere), edit this function so that dead agents are removed correctly without leaking memory
};