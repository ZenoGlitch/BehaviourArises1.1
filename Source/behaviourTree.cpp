#include "behaviourTree.h"

#include "level.h"

BehaviourTree::BehaviourTree()
{

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
	for (auto *child : getChildren())
	{
		child->run(level, agent);
		state = child->state;

		if (state == success || state == running )
		{
			return state;
		}
	}
	state = failure;
	return state;
}

bool BehaviourTree::Sequence::run(Level* level, Agent* agent)
{
	for (auto *child : getChildren()) 
	{
		
		child->run(level, agent);
		state = child->state;

		if (state == failure || state == running)
		{
			return state;
		}
	}
	state = success;
	return state;
}

bool BehaviourTree::DecoratorConditional::run(Level* level, Agent* agent)
{
	if (condition)
	{
		state = success;
		return state;
	}
	else
	{
		state = failure;
		return state;
	}
}

void BehaviourTree::DecoratorConditional::setCondition(bool p_condition)
{
	condition = p_condition;
}

bool BehaviourTree::DecoratorSelector::run(Level* level, Agent* agent)
{
	if (condition)
	{
		for (auto* child : getChildren())
		{
			child->run(level, agent);
			state = child->state;

			if (state == success || state == running)
			{
				return state;

			}
		}
		state = failure;
		return state;
	}
	else
	{
		state = failure;
		return state;
	}
}

void BehaviourTree::DecoratorSelector::setCondition(bool p_condition)
{
	condition = p_condition;
}

bool BehaviourTree::DecoratorSequence::run(Level* level, Agent* agent)
{
	if (condition)
	{
		for (auto* child : getChildren()) 
		{
			child->run(level, agent);
			state = child->state;

			if (state == failure || state == running)
			{
				return state;
			}
		}
		state = success;
		return state;
	}
	state = success;
	return state;
}

BehaviourTree::DecoratorAction::DecoratorAction(int p_actionId)
	: actionId(p_actionId)
{
}

bool BehaviourTree::DecoratorAction::run(Level* level, Agent* agent)
{
	if (actionId == level->tank_moveToLowestHealthAlly_id && level->healer.alive)
	{
		Vector2 tankPos = level->tank.getPosition();
		Vector2 healerPos = level->healer.getPosition();
		Vector2 playerPos = level->player.getPosition();
		float distanceToHealer = Vector2Distance(tankPos, healerPos);
		float distanceToPlayer = Vector2Distance(tankPos, playerPos);

		if (level->tank_moveToLowestHealthAlly.condition) // healer has lowest health
		{
			level->moveAgentTowardsOtherAgent(level->tank, level->healer.getPosition());
			if ( distanceToHealer <= 65)
			{
				level->tank_checkAlliesHealth.condition = false;
				level->tank_moveToLowestHealthAlly.condition = false;
				level->tank.closeToAlly = true;
			}
			state = running;
			return state;
		}
		else // player has lowest health
		{
			level->moveAgentTowardsOtherAgent(level->tank, level->player.getPosition());
			if (distanceToPlayer <= 65)
			{
				level->tank_checkAlliesHealth.condition = false;
				level->tank_moveToLowestHealthAlly.condition = false;
				level->tank.closeToAlly = true;

			}
			state = running;
			return state;
		}
	}

	state = failure;
	return state;
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

BehaviourTree::Action::Action(int p_actionId) : actionId(p_actionId)
{
}

bool BehaviourTree::Action::run(Level *level, Agent* agent)
{

	// TANK BT ACTIONS
	if (actionId == level->tank_moveToHealer_id && level->healer.alive)
	{
		Vector2 tankPos = level->tank.getPosition();
		Vector2 healerPos = level->healer.getPosition();
		level->moveAgentTowardsOtherAgent(level->tank, healerPos);
		state = running;
		return state;
	}

	if (actionId == level->tank_moveToMonster_id)
	{

		Vector2 monsterPos = level->tank.getClosestMonsterPos(level);
		Vector2 targetPos = level->tank.getTargetPos();
		level->moveAgentTowardsOtherAgent(level->tank, targetPos);
		state = running;
		return state;
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

					level->tank.attackCooldown = 3.0f;
					state = success;
				}
			}
		}
		return state;
	}
	

	// HEALER BT ACTIONS
	if (actionId == level->healer_runAway_id)
	{
		level->healer.drawHealCircle = false;
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;

		Vector2 healerPos = level->healer.getPosition();
		Vector2 tankPos = level->tank.getPosition();
		
		if (level->tank.alive)
		{
			level->moveAgentTowardsOtherAgent(level->healer, tankPos);
		}

		level->healingCooldown -= GetFrameTime();

		if (healerPos.x <= tankPos.x + 60.0f && healerPos.x >= tankPos.x - 60.0f
			&& healerPos.y <= tankPos.y + 60.0f && healerPos.y >= tankPos.y - 60.0f
			&& level->healingCooldown < 0.0f)
		{
			level->healer.heal(20.0f);
			level->healer.setTargetPos(healerPos);
			level->healer.drawHealCircle = true;
			level->healingCooldown = 2.0f;
		}

		state = running;
		return state;
	}

	if (actionId == level->healer_moveToLowestHealthAlly_id)
	{
		level->healer.drawHealCircle = false;
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;
		Vector2 targetPos = level->healer.getTargetPos();
		level->moveAgentTowardsOtherAgent(level->healer, targetPos);
		state = running;
		return state;
	}

	if (actionId == level->healer_healTarget_id)
	{
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;
		level->healer.drawHealCircle = true;

		const float healAmount = 25.0f;
		level->healingCooldown -= GetFrameTime();
		if (level->healer.target == Agent::Target::Tank && level->healingCooldown <= 0.0f && level->tank.alive)
		{
			level->tank.heal(healAmount);
			level->healer.drawHealCircle = false;
			level->healingCooldown = 2.0f;
			state = running;
			return state;
		}
		else
		{
			level->healer.drawHealCircle = false;
		}

		if (level->healer.target == Agent::Target::Player && level->healingCooldown <= 0.0f)
		{
			level->player.heal(healAmount);
			level->healer.drawHealCircle = false;
			level->healingCooldown = 2.0f;
			state = running;
			return state;
		}
		else
		{
			level->healer.drawHealCircle = false;
		}

		state = success;
		return state;
	}
	else
	{
		level->healer.drawHealCircle = false;
	}

	if (actionId == level->healer_moveToMonster_id)
	{
		level->healer.projectile.active = false;
		level->healer.projectile.positionsSet = false;
		level->healer.drawHealCircle = false;

		Vector2 monsterPos = level->getClosestMonsterPos(&level->healer);
		level->moveAgentTowardsOtherAgent(level->healer, monsterPos);

		state = running;
		return state;
	}

	if (actionId == level->healer_attack_id)
	{
		level->healer.drawHealCircle = false;
		level->healer.shoot(level);
		state = success;
		return state;
	}


	// MONSTER BT ACTIONS
	for (auto& monsters : level->monsterAgents)
	{
		if (level->monster_checkOwnHealth.condition)
		{
			if (actionId == level->monster_runAway_id)
			{
				Vector2 currentTarget = monsters.getTargetPos();
				Vector2 monsterPos = monsters.getPosition();
				Vector2 newTarget = {100000, 100000};
				if (monsterPos.x < currentTarget.x)
				{
					newTarget.x = 0;
				}
				else
				{
					newTarget.x = (float)GetScreenWidth();
				}
				if (monsterPos.y < currentTarget.y)
				{
					newTarget.y = 0;
				}
				else
				{
					newTarget.y = (float)GetScreenHeight();
				}
				monsters.setTargetPos(newTarget);
				monsters.moveTowards(newTarget);
			}

			state = running;
			return state;
		}
	}

	if (agent != nullptr && agent->type == Agent::type_monster)
	{
		if (!agent->inAttackRange)
		{
			if (actionId == level->monster_moveToClosestTarget_id)
			{
				Vector2 monsterPos = agent->getPosition();
				Vector2 targetPos = agent->getTargetPos();

				level->moveAgentTowardsOtherAgent(*agent, targetPos);
				state = running;
				return state;
			}
		}
		else if (agent->inAttackRange)
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

				state = running;
				return state;
			}
		}
		state = failure;
		return state;
	}

	state = failure;
	return state;
}