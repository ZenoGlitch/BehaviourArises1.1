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


	// Tank BT stuff
	enum TankActionId
	{
		//tank_checkOwnHealthId,
		//tank_checkAllyHealthId,
		tank_moveToHealer_Id,
		tank_moveToPlayer_Id,
		tank_moveToLowestHealthAlly_Id,
		tank_moveToMonster_Id,
		tank_attackId,
		tank_count
	};

	BehaviourTree::Sequence tank_sequence[2];
	BehaviourTree::Selector tank_selector[2];
	BehaviourTree::DecoratorConditional tank_checkOwnHealth;
	BehaviourTree::DecoratorConditional tank_checkAlliesHealth;
	BehaviourTree::DecoratorAction tank_moveToLowestHealthAlly = BehaviourTree::DecoratorAction(tank_moveToLowestHealthAlly_Id);
	Action tank_moveTowardsHealer = Action(tank_moveToHealer_Id);

	Action tank_moveTowardsMonster = Action(tank_moveToMonster_Id);

	//Action moveTowardsPlayer = Action(tank_moveToPlayer_Id);

	// End of Tank BT stuff
	

	// Healer BT stuff
	enum HealerActionId
	{
		healer_checkOwnHealth_id = tank_count + 1,
		healer_runAway_id,
		healer_checkAllyHealth_id,
		healer_moveToLowestHealthAlly_id,
		healer_healTarget_id,
		healer_moveToMonster_id,
		healer_attack_id,
		healer_count
	};

	BehaviourTree::Sequence healer_sequence[6];
	BehaviourTree::Selector healer_selector[3];
	BehaviourTree::DecoratorConditional healer_checkOwnHealth;
	Action healer_runAway = Action(healer_runAway_id);
	BehaviourTree::DecoratorConditional healer_checkAlliesHealth;
	BehaviourTree::DecoratorConditional healer_notInHealingRange;
	Action healer_moveToLowestHealthAlly = Action(healer_moveToLowestHealthAlly_id);
	Action healer_healTarget = Action(healer_healTarget_id);
	BehaviourTree::DecoratorConditional healer_inHealrange;
	BehaviourTree::DecoratorConditional healer_notInAttackRange;
	Action healer_moveToMonster = Action(healer_moveToMonster_id);
	BehaviourTree::DecoratorConditional healer_inAttackRange;
	Action healer_attack = Action(healer_attack_id);





	// End of Healer BT stuff


	// Monster BT stuff
	enum MonsterActionId
	{
		monster_checkOwnHealth_id = healer_count + 1,
		monster_runAway_id,
		monster_moveToClosestTarget_id,
		monster_attack_id,
	};

	BehaviourTree::Sequence monster_sequence[3];
	BehaviourTree::Selector monster_selector;
	BehaviourTree::DecoratorConditional monster_checkOwnHealth;
	Action monster_runAway = Action(monster_runAway_id);
	BehaviourTree::DecoratorConditional monster_notInAttackRange;
	Action monster_moveToClosestTarget = Action(monster_moveToClosestTarget_id);
	BehaviourTree::DecoratorConditional monster_inAttackRange;
	Action monster_attack = Action(monster_attack_id);

	// End of Monster BT stuff

	//std::vector<Monster*> monsterAgents;
	std::list<Monster> monsterAgents;


	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame

	float healingCooldown = 2;
	int killCounter = 0;
	int maxMonsterCount = 2;


private:
	int last_id = 0;

	//NOTE(Filippo): Using a list here is not the best idea, ideally you should store agents in some other data structure that keeps them close to each other while being pointer-stable.
	//std::list<SillyAgent> silly_agents;
	// @AddMoreHere

	std::unordered_map<int, Agent*> id_to_agent;
	std::vector<Agent*> all_agents;

	Texture background;

	float damageCooldown = 1;
	bool damageTaken = false;

public:

	Level() = default;

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

	void damageMonstersWithPlayersSword();
	void damageAgent(Agent &p_agent);
	void moveAgentTowardsOtherAgent(Agent &agentToMove, Vector2 targetAgentPos);
	Vector2 getClosestMonsterPos(Agent* agent);

private:
	void remove_dead_and_add_pending_agents();
	// Remember, if you add more lists (see @AddMoreHere), edit this function so that dead agents are removed correctly without leaking memory
};