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

	Texture playerTex;
	Texture swordTex;
	Texture monsterTex;
	Texture tankTex;
	Texture healerTex;
	Texture projectileTex;


	// Tank BT stuff
	enum TankActionId
	{
		tank_moveToHealer_id,
		tank_moveToLowestHealthAlly_id,
		tank_moveToMonster_id,
		tank_attack_id,
		tank_count
	};

	BehaviourTree::Sequence tank_sequence[4];
	BehaviourTree::Selector tank_selector[2];
	BehaviourTree::DecoratorConditional tank_checkOwnHealth;
	BehaviourTree::DecoratorConditional tank_checkAlliesHealth;
	BehaviourTree::DecoratorAction tank_moveToLowestHealthAlly = BehaviourTree::DecoratorAction(tank_moveToLowestHealthAlly_id);
	BehaviourTree::Action tank_moveTowardsHealer = BehaviourTree::Action(tank_moveToHealer_id);
	BehaviourTree::DecoratorConditional tank_notInAttackRange;
	BehaviourTree::DecoratorConditional tank_inAttackRange;
	BehaviourTree::Action tank_moveTowardsMonster = BehaviourTree::Action(tank_moveToMonster_id);
	BehaviourTree::Action tank_attack = BehaviourTree::Action(tank_attack_id);

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
	BehaviourTree::Action healer_runAway = BehaviourTree::Action(healer_runAway_id);
	BehaviourTree::DecoratorConditional healer_checkAlliesHealth;
	BehaviourTree::DecoratorConditional healer_notInHealingRange;
	BehaviourTree::Action healer_moveToLowestHealthAlly = BehaviourTree::Action(healer_moveToLowestHealthAlly_id);
	BehaviourTree::Action healer_healTarget = BehaviourTree::Action(healer_healTarget_id);
	BehaviourTree::DecoratorConditional healer_inHealrange;
	BehaviourTree::DecoratorConditional healer_notInAttackRange;
	BehaviourTree::Action healer_moveToMonster = BehaviourTree::Action(healer_moveToMonster_id);
	BehaviourTree::DecoratorConditional healer_inAttackRange;
	BehaviourTree::Action healer_attack = BehaviourTree::Action(healer_attack_id);





	// End of Healer BT stuff


	// Monster BT stuff
	enum MonsterActionId
	{
		monster_checkOwnHealth_id = healer_count + 1,
		monster_getKnockedBack_id,
		monster_runAway_id,
		monster_moveToClosestTarget_id,
		monster_attack_id,
	};

	BehaviourTree::Sequence monster_sequence[4];
	BehaviourTree::Selector monster_selector;
	BehaviourTree::DecoratorConditional monster_attackedByTank;
	BehaviourTree::Action monster_getKnockedBack = BehaviourTree::Action(monster_getKnockedBack_id);
	BehaviourTree::DecoratorConditional monster_checkOwnHealth;
	BehaviourTree::Action monster_runAway = BehaviourTree::Action(monster_runAway_id);
	BehaviourTree::DecoratorConditional monster_notInAttackRange;
	BehaviourTree::Action monster_moveToClosestTarget = BehaviourTree::Action(monster_moveToClosestTarget_id);
	BehaviourTree::DecoratorConditional monster_inAttackRange;
	BehaviourTree::Action monster_attack = BehaviourTree::Action(monster_attack_id);

	// End of Monster BT stuff

	//std::vector<Monster*> monsterAgents;
	std::list<Monster> monsterAgents;


	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame

	float healingCooldown = 2;
	int killCounter = 0;
	int maxMonsterCount = 2;

	float updateTick = 0;

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

	void loadTextures();

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