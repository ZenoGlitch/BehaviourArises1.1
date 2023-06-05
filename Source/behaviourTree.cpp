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
	// // O: insert return statement here
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

bool BehaviourTree::Selector::run(Level* level)
{
	for (auto *child : getChildren())   // The generic Selector implementation
	{
		if (child->run(level))  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
		{
			return true;

		}
	}
	return false;  // All children failed so the entire run() operation fails.
}

bool BehaviourTree::Sequence::run(Level* level)
{
	for (auto *child : getChildren()) {  // The generic Sequence implementation.
		if (!child->run(level))  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
			return false;
	}
	return true;  // All children suceeded, so the entire run() operation succeeds.
}

bool BehaviourTree::DecoratorConditional::run(Level* level)
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

bool BehaviourTree::DecoratorSelector::run(Level* level)
{
	if (condition)
	{
		for (auto* child : getChildren())   // The generic Selector implementation
		{
			if (child->run(level))  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
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

bool BehaviourTree::DecoratorSequence::run(Level* level)
{
	if (condition)
	{
		for (auto* child : getChildren()) {  // The generic Sequence implementation.
			if (!child->run(level))  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
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

bool BehaviourTree::DecoratorAction::run(Level* level)
{
	if (actionId == level->tank_moveTankTowardsLowestHealthAllyId)
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

bool BehaviourTree::Root::run(Level* level)
{
	if (child != nullptr)
	return child->run(level);
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

bool Action::run(Level *level)
{


	// TANK BT ACTIONS
	
	//if (name == "tankCheckOwnHealth")
	//{
	//	float health = level->tank.getEnergy();
	//	if (health < level->tank.maxEnergy / 2)
	//	{
	//		level->tank.state = level->tank.runningAway;
	//	}
	//}

	//if (name == "moveTowardsPlayer") // move Tank towards Player
	if (actionId == level->tank_moveTankTowardsPlayerId)
	{
		//Vector2 pos = level->tank.getPosition();
		//Vector2 targetPos = level->player.getPosition();

		////Calulate angle
		//Vector2 diff = Vector2Subtract(pos, targetPos);
		//float angle = -atan2f(diff.x, diff.y) * RAD2DEG;
		////rotate towards player
		//level->tank.setRotation(angle);


		////move towards player
		//if (pos.x < targetPos.x)
		//{
		//	pos.x += level->tank.moveSpeed * GetFrameTime();
		//}
		//if (pos.x > targetPos.x)
		//{
		//	pos.x -= level->tank.moveSpeed * GetFrameTime();
		//}
		//if (pos.y < targetPos.y)
		//{
		//	pos.y += level->tank.moveSpeed * GetFrameTime();
		//}
		//if (pos.y > targetPos.y)
		//{
		//	pos.y -= level->tank.moveSpeed * GetFrameTime();
		//}

		//level->tank.setPosition(pos);

		level->moveAgentTowardsOtherAgent(level->tank, level->player.getPosition());

		return true;
	}


	//if (name == "moveTowardsHealer") // move Tank towards Healer
	if (actionId == level->tank_moveTankTowardsHealerId)
	{
		//Vector2 pos = level->tank.getPosition();
		//Vector2 targetPos = level->healer.getPosition();

		////Calulate angle
		//Vector2 diff = Vector2Subtract(pos, targetPos);
		//float angle = -atan2f(diff.x, diff.y) * RAD2DEG;
		////rotate towards healer
		//level->tank.setRotation(angle);


		////move towards healer
		//if (pos.x < targetPos.x)
		//{
		//	pos.x += level->tank.moveSpeed * GetFrameTime();
		//}
		//if (pos.x > targetPos.x)
		//{
		//	pos.x -= level->tank.moveSpeed * GetFrameTime();
		//}
		//if (pos.y < targetPos.y)
		//{
		//	pos.y += level->tank.moveSpeed * GetFrameTime();
		//}
		//if (pos.y > targetPos.y)
		//{
		//	pos.y -= level->tank.moveSpeed * GetFrameTime();
		//}

		//level->tank.setPosition(pos);

		level->moveAgentTowardsOtherAgent(level->tank, level->healer.getPosition());
		return true;
	}
	

	// HEALER BT ACTIONS

	// MONSTER BT ACTIONS

	return false;
}

//TankBT::TankBT()
//{
////	TankBT::tBT.setRootChild(&selector[0]);
////	TankBT::selector[0].addChild(&checkOwnHealth);
////	TankBT::selector[0].addChild(&moveTowardsPlayer);
////	
//}
//
////const TankBT::Selector &TankBT::GetRoot()
////{
////	//return TankBT::selector1;
////}
//
//void TankBT::setRootChild(Selector* rootChild)
//{
//	tBT_root->setChild(rootChild);
//}