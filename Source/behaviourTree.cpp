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
	if (actionId == level->tank_moveToLowestHealthAlly_id)
	{

		if (level->tank_moveToLowestHealthAlly.condition) // healer has lowest health
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

	if (actionId == level->tank_moveToHealer_id)
	{
		Vector2 healerPos = level->healer.getPosition();
		level->moveAgentTowardsOtherAgent(level->tank, healerPos);
		return true;
	}

	if (actionId == level->tank_moveToMonster_id)
	{
		Vector2 monsterPos = level->tank.getClosestMonsterPos(level);
		level->moveAgentTowardsOtherAgent(level->tank, monsterPos);
		return true;
	}

	if (actionId == level->tank_attack_id)
	{
		level->tank.attackCooldown -= GetFrameTime();
		if (level->tank.attackCooldown <= 0.0f)
		{
			for (auto& monster : level->monsterAgents)
			{
				Vector2 monsterPos = monster.getPosition();
				Vector2 tankPos = level->tank.getPosition();
				float distanceToMonster = Vector2Distance(tankPos, monsterPos);
				if (distanceToMonster <= 150)
				{
					monster.damage(30);

					//monster.recieveKnockback = true;

					level->tank.attackCooldown = 3.5f;

				}
			}
		}
		return true;
	}
	

	// HEALER BT ACTIONS
	if (actionId == level->healer_runAway_id)
	{
		level->healer.drawHealCircle = false;
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;
		//Vector2 healerPos = level->healer.getPosition();
		Vector2 tankPos = level->tank.getPosition();
		level->moveAgentTowardsOtherAgent(level->healer, tankPos);
		return true;
	}

	if (actionId == level->healer_moveToLowestHealthAlly_id)
	{
		level->healer.drawHealCircle = false;
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;
		Vector2 targetPos = level->healer.getTargetPos();
		level->moveAgentTowardsOtherAgent(level->healer, targetPos);
		return true;
	}

	if (actionId == level->healer_healTarget_id)
	{
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;

		float healAmount = 25;
		level->healingCooldown -= GetFrameTime();
		level->healer.drawHealCircle = true;
		if (level->healer.target == Agent::Target::Tank && level->healingCooldown <= 0)
		{
			level->healer.drawHealCircle = true;
			level->tank.heal(healAmount);
			level->healingCooldown = 2;
			return true;
		}
		//else
		//{
		//	level->healer.drawHealCircle = false;
		//}

		if (level->healer.target == Agent::Target::Player && level->healingCooldown <= 0)
		{
			level->healer.drawHealCircle = true;
			level->player.heal(healAmount);
			level->healingCooldown = 2;
			return true;
		}
		//else
		//{
		//	level->healer.drawHealCircle = false;
		//}

		//level->healer.drawHealCircle = false;

		return true;
	}


	if (actionId == level->healer_moveToMonster_id)
	{
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;
		level->healer.drawHealCircle = false;

		//Vector2 healerPos = level->healer.getPosition();
		//Vector2 monsterPos;
		//float lowestDistance = 100000000;
		//for (auto& monster : level->monsterAgents)
		//{
		//	monsterPos = monster.getPosition();
		//	float distance = Vector2Distance(healerPos, monsterPos );
		//	if (distance < lowestDistance)
		//	{
		//		lowestDistance = distance;
		//	}
		//}

		Vector2 monsterPos = level->getClosestMonsterPos(&level->healer);
		level->moveAgentTowardsOtherAgent(level->healer, monsterPos);

		return true;
	}

	if (actionId == level->healer_attack_id)
	{
		level->healer.drawHealCircle = false;
		level->healer.shoot(level);
		//printf("healer should be attacking\n");
		return true;
	}





	// MONSTER BT ACTIONS

	//for (auto& monster : level->monsterAgents)
	//{
	//	if (agent->type == Agent::type_monster && actionId == level->monster_getKnockedBack_id)
	//	{
	//		Vector2 monsterPos = monster.getPosition();
	//		Vector2 tankPos = level->tank.getPosition();
	//		if (monsterPos.x < tankPos.x && monsterPos.y < tankPos.y)
	//		{
	//			Vector2 landingPos = { monsterPos.x - 200, monsterPos.y - 200 };
	//			Vector2 travelPos = Vector2Lerp(monsterPos, landingPos, 0.2);
	//			monster.setPosition(travelPos);
	//
	//		}
	//		if (monsterPos.x > tankPos.x && monsterPos.y > tankPos.y)
	//		{
	//			Vector2 landingPos = { monsterPos.x + 200, monsterPos.y + 200 };
	//			Vector2 travelPos = Vector2Lerp(monsterPos, landingPos, 0.2);
	//			monster.setPosition(travelPos);
	//		}
	//		if (monsterPos.x < tankPos.x && monsterPos.y > tankPos.y)
	//		{
	//			Vector2 landingPos = { monsterPos.x - 200, monsterPos.y + 200 };
	//			Vector2 travelPos = Vector2Lerp(monsterPos, landingPos, 0.2);
	//			monster.setPosition(travelPos);
	//		}
	//		if (monsterPos.x > tankPos.x && monsterPos.y < tankPos.y)
	//		{
	//			Vector2 landingPos = { monsterPos.x + 200, monsterPos.y - 200 };
	//			Vector2 travelPos = Vector2Lerp(monsterPos, landingPos, 0.2);
	//			monster.setPosition(travelPos);
	//		}
	//	}
	//		return true;
	//}

	//for (auto &monster : level->monsterAgents)
	//{
	//	if (actionId == level->monster_getKnockedBack_id)
	//	{
	//		Vector2 landingPos = {100000, 100000};
	//		Vector2 monsterPos = monster.getPosition();
	//		Vector2 tankPos = level->tank.getPosition();
	//		float distanceToTank = Vector2Distance(monsterPos, tankPos);
	//		if (distanceToTank <= 150)
	//		{
	//			if (!monster.isPositionsSet)
	//			{
	//				if (monsterPos.x < tankPos.x)
	//				{
	//					landingPos.x = monsterPos.x - 200;
	//				}
	//				if (monsterPos.x > tankPos.x)
	//				{
	//					landingPos.x = monsterPos.x + 200;
	//				}
	//				if (monsterPos.y < tankPos.y)
	//				{
	//					landingPos.y = monsterPos.y - 200;
	//				}
	//				if (monsterPos.y > tankPos.y)
	//				{
	//					landingPos.y = monsterPos.y + 200;
	//				}
	//				//monster.setTargetPos(landingPos);
	//				monster.isPositionsSet = true;
	//			}
	//		}
	//		if (landingPos.x <= 0)
	//		{
	//			landingPos.x = 0;
	//		}
	//		if (landingPos.y <= 0)
	//		{
	//			landingPos.y = 0;
	//		}
	//		if (landingPos.x >= GetScreenWidth())
	//		{
	//			landingPos.x = GetScreenWidth();
	//		}
	//		if (landingPos.y >= GetScreenHeight())
	//		{
	//			landingPos.y = GetScreenHeight();
	//		}
	//		Vector2 flightPos = Vector2Lerp(monsterPos, landingPos, 0.03);
	//		monster.setPosition(flightPos);
	//		
	//	}
	//}

	for (auto& monster : level->monsterAgents)
	{
		if (actionId == level->monster_getKnockedBack_id)
		{
			Vector2 landingPos = { 100000, 100000 };
			Vector2 monsterPos = agent->getPosition();
			Vector2 tankPos = level->tank.getPosition();
			float distanceToTank = Vector2Distance(monsterPos, tankPos);
			if (distanceToTank <= 150)
			{
				float launchDistance = 300;
				if (!monster.isPositionsSet)
				{
					if (monsterPos.x < tankPos.x)
					{
						landingPos.x = monsterPos.x - launchDistance;
					}
					if (monsterPos.x > tankPos.x)
					{
						landingPos.x = monsterPos.x + launchDistance;
					}
					if (monsterPos.y < tankPos.y)
					{
						landingPos.y = monsterPos.y - launchDistance;
					}
					if (monsterPos.y > tankPos.y)
					{
						landingPos.y = monsterPos.y + launchDistance;
					}

					//monster.setTargetPos(landingPos);
					monster.isPositionsSet = true;
				}

				if (landingPos.x <= 0)
				{
					landingPos.x = 0;
				}
				if (landingPos.y <= 0)
				{
					landingPos.y = 0;
				}
				if (landingPos.x >= GetScreenWidth())
				{
					landingPos.x = GetScreenWidth();
				}
				if (landingPos.y >= GetScreenHeight())
				{
					landingPos.y = GetScreenHeight();
				}

				Vector2 flightPos = Vector2Lerp(monsterPos, landingPos, 0.01);
				agent->setPosition(flightPos);
				
				Vector2 currentPos = agent->getPosition();
				if (   currentPos.x >= landingPos.x - 10
					&& currentPos.x <= landingPos.x + 10
					&& currentPos.y >= landingPos.y - 10
					&& currentPos.y <= landingPos.y + 10 )
				{
					monster.isPositionsSet = false;
					monster.recieveKnockback = false;
					level->monster_attackedByTank.condition = false;
				}
			}


			return true;
		}
	}

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
		float monsterAttackDamage = 10;
		if (actionId == level->monster_attack_id)
		{
			if (agent->attackCooldown <= 0.0f)
			{
				if (agent->target == agent->Healer)
				{
					level->healer.damage(monsterAttackDamage);
					agent->attackCooldown = agent->maxAttackCooldown;
				}
				if (agent->target == agent->Tank)
				{
					level->tank.damage(monsterAttackDamage);
					agent->attackCooldown = agent->maxAttackCooldown;
				}
				if (agent->target == agent->Player)
				{
					level->player.damage(monsterAttackDamage);
					agent->attackCooldown = agent->maxAttackCooldown;
				}

			}
			agent->attackCooldown -= GetFrameTime();

			return true;
		}
	}

	return false;
}