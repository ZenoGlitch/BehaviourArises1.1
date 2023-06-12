#include "behaviourTree.h"

#include "level.h"

BehaviourTree::BehaviourTree() : root(new Root)
{

}

void BehaviourTree::setRootChild(Selector* rootChild)
{
	root->setChild(rootChild);
}

const std::vector<BehaviourTree::Node*>& BehaviourTree::CompositeNode::getChildren()
{
	 return children;
}

void BehaviourTree::CompositeNode::addChild(Node* child)
{
	children.emplace_back(child);
}

void BehaviourTree::CompositeNode::addChildren(std::initializer_list<Node*>&& newChildren)
{
	for (Node* child : newChildren)
	{
		addChild(child);
	}
}

bool BehaviourTree::Selector::run(Level* level, Agent* agent)
{
	for (auto *child : getChildren())   // The generic Selector implementation
	{
		if (child->run(level, agent))  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
		{
			return true;

		}
	}
	return false;  // All children failed so the entire run() operation fails.
}

bool BehaviourTree::Sequence::run(Level* level, Agent* agent)
{
	for (auto *child : getChildren()) {  // The generic Sequence implementation.
		if (!child->run(level, agent))  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
			return false;
	}
	return true;  // All children suceeded, so the entire run() operation succeeds.
}

bool BehaviourTree::DecoratorConditional::run(Level* level, Agent* agent)
{
	if (condition)
	{
		return true;
	}
	else return false;
}

void BehaviourTree::DecoratorConditional::setCondition(bool p_condition)
{
	condition = p_condition;
}

bool BehaviourTree::DecoratorSelector::run(Level* level, Agent* agent)
{
	if (condition)
	{
		for (auto* child : getChildren())   // The generic Selector implementation
		{
			if (child->run(level, agent))  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
			{
				return true;

			}
		}
		return false;  // All children failed so the entire run() operation fails.
	}
	else return false;
}

void BehaviourTree::DecoratorSelector::setCondition(bool p_condition)
{
	condition = p_condition;
}

bool BehaviourTree::DecoratorSequence::run(Level* level, Agent* agent)
{
	if (condition)
	{
		for (auto* child : getChildren()) {  // The generic Sequence implementation.
			if (!child->run(level, agent))  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
				return false;
		}
		return true;  // All children suceeded, so the entire run() operation succeeds.
	}
	return false;
}

BehaviourTree::DecoratorAction::DecoratorAction(std::string newName, int prob)
	: name(newName), probabilityOfSuccess(prob)
{
}

BehaviourTree::DecoratorAction::DecoratorAction(int p_actionId)
	: actionId(p_actionId)
	, name("name")
	, probabilityOfSuccess(100)
{
}

bool BehaviourTree::DecoratorAction::run(Level* level, Agent* agent)
{
	if (actionId == level->tank_moveToLowestHealthAlly_Id)
	{
		if (condition) // healer has lowest health
		{
			level->moveAgentTowardsOtherAgent(level->tank, level->healer.getPosition());
			return true;
		}
		else // player has lowest health
		{
			level->moveAgentTowardsOtherAgent(level->tank, level->player.getPosition());
			return true;
		}
	}

	return false;
}

void BehaviourTree::DecoratorAction::setCondition(bool p_condition)
{
	condition = p_condition;
}

void BehaviourTree::DecoratorSequence::setCondition(bool p_condition)
{
	condition = p_condition;
}

void BehaviourTree::Root::setChild(Selector *newChild)
{
	child = newChild;
}

bool BehaviourTree::Root::run(Level* level, Agent* agent)
{
	if (child != nullptr)
	return child->run(level, agent);
	else return false;

}

Action::Action(std::string newName, int prob) 
	: name(newName), probabilityOfSuccess(prob)
{
}

Action::Action(int p_actionId)
	:actionId(p_actionId)
	,probabilityOfSuccess(100)
	,name("name")
{
}

bool Action::run(Level *level, Agent* agent)
{

	// TANK BT ACTIONS

	if (actionId == level->tank_moveToPlayer_Id)
	{
		Vector2 playerPos = level->player.getPosition();
		level->moveAgentTowardsOtherAgent(level->tank, playerPos);
		return true;
	}

	if (actionId == level->tank_moveToHealer_Id)
	{
		Vector2 healerPos = level->healer.getPosition();
		level->moveAgentTowardsOtherAgent(level->tank, healerPos);
		return true;
	}

	if (actionId == level->tank_moveToMonster_Id)
	{
		Vector2 monsterPos = level->tank.getClosestMonsterPos(level);
		level->moveAgentTowardsOtherAgent(level->tank, monsterPos);
		return true;
	}
	

	// HEALER BT ACTIONS

	// MONSTER BT ACTIONS

	for (auto& monsters : level->monsterAgents)
	{
		if (level->monster_checkOwnHealth.condition)
		{
			if (actionId == level->monster_runAway_id)
			{
				//printf("MONSTER RUNNING AWAY!!\n");
				Vector2 currentTarget = monsters.getTargetPos();
				Vector2 monsterPos = monsters.getPosition();
				Vector2 newTarget;
				if (monsterPos.x < currentTarget.x)
				{
					newTarget.x = 0;
				}
				else
				{
					newTarget.x = GetScreenWidth();
				}
				if (monsterPos.y < currentTarget.y)
				{
					newTarget.y = 0;
				}
				else
				{
					newTarget.y = GetScreenHeight();
				}
				monsters.setTargetPos(newTarget);
				monsters.moveTowards(newTarget);
			}
			return true;
		}
	}

	if (agent->type == Agent::type_monster)
	{
		if (!agent->inAttackRange)
		{
			if (actionId == level->monster_moveToClosestTarget_id)
			{
				//printf("moving to target\n");
				Vector2 monsterPos = agent->getPosition();
				Vector2 targetPos = agent->getTargetPos();

				level->moveAgentTowardsOtherAgent(*agent, targetPos);

				return true;
			}
		}
	}

	if (/*agent != nullptr &&*/ agent->type == Agent::type_monster && agent->inAttackRange)
	{
		if (actionId == level->monster_attack_id)
		{
			if (agent->attackCooldown <= 0.0f)
			{
				if (agent->target == agent->Healer)
				{
					float healerHealth = level->healer.getEnergy();
					agent->attackCooldown = agent->maxAttackCooldown;
				}
				if (agent->target == agent->Tank)
				{
					level->tank.damage(5);
					agent->attackCooldown = agent->maxAttackCooldown;
				}
				if (agent->target == agent->Player)
				{
					level->player.damage(5);
					agent->attackCooldown = agent->maxAttackCooldown;
				}

			}
			agent->attackCooldown -= GetFrameTime();

			return true;
		}
	}

	return false;
}