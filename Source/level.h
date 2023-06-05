#pragma once
#include "raylib.h"

#include "agent.h"
#include "behaviourTree.h"
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
	TestAgent player;
	Monster monster;
	Tank tank;
	Healer healer;

	//TankBT tBt;

	// Tank BT stuff
	enum TankActionId
	{
		//tank_checkOwnHealthId,
		//tank_checkAllyHealthId,
		tank_moveTankTowardsHealerId,
		tank_moveTankTowardsPlayerId,
		tank_moveTankTowardsLowestHealthAllyId,
		tank_moveTankTowardsMonsterId,
		tank_attackId,

	};

	BehaviourTree::Sequence tank_sequence[2];
	BehaviourTree::Selector tank_selector[2];
	BehaviourTree::DecoratorConditional tank_checkOwnHealth;
	BehaviourTree::DecoratorConditional tank_checkAlliesHealth;
	BehaviourTree::DecoratorAction tank_moveToLowestHealthAlly = BehaviourTree::DecoratorAction(tank_moveTankTowardsLowestHealthAllyId);
	Action moveTowardsHealer = Action(tank_moveTankTowardsHealerId);
	Action moveTowardsPlayer = Action(tank_moveTankTowardsPlayerId);

	//Action moveTowardsHealer = Action("moveTowardsHealer", 100);
	//Action moveTowardsPlayer = Action("moveTowardsPlayer", 100);
	//Action checkOwnHealth = Action("tankCheckOwnHealth", 100);

	// End of Tank BT stuff
	

	//std::vector<Monster*> monsterAgents;
	std::list<Monster> monsterAgents;


	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame

private:
	int last_id = 0;

	//NOTE(Filippo): Using a list here is not the best idea, ideally you should store agents in some other data structure that keeps them close to each other while being pointer-stable.
	//std::list<SillyAgent> silly_agents;
	// @AddMoreHere

	std::unordered_map<int, Agent*> id_to_agent;
	std::vector<Agent*> all_agents;

	Texture background;

	float damageCooldown = 2;
	bool damageTaken = false;

public:
	Agent* get_agent(int id);

	//Agent* spawn_agent(SillyAgent agent);


	Monster* spawnMonster(Monster monster);
	// @AddMoreHere

	void initialize();
	void initialize_behaviour_tree();

	void input();

	void reset();
	void update();
	void draw();

	void moveAgentTowardsOtherAgent(Agent &agentToMove, Vector2 targetAgentPos);

private:
	void remove_dead_and_add_pending_agents();
	// Remember, if you add more lists (see @AddMoreHere), edit this function so that dead agents are removed correctly without leaking memory
};