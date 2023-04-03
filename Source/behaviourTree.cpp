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

void BehaviourTree::Root::setChild(Selector *newChild)
{
	child = newChild;
}

bool BehaviourTree::Root::run(Level* level)
{

	//return child->run();
	return false;

}

Action::Action(std::string newName, int prob) 
	: name(newName), probabilityOfSuccess(prob)
{
}

bool Action::run(Level *level)
{
	// GENERIC BT ACTIONS
	if (name == "testing1")
	{
		//printf("test 1 succeeded\n");
		return true;
	}





	// TANK BT ACTIONS
	if (name == "tankCheckOwnHealth")
	{
		float health = level->tank.getEnergy();
		if (health < level->tank.maxEnergy / 2)
		{
			level->tank.state = level->tank.runningAway;
		}
	}

	if (name == "moveTowardsPlayer") // move Tank towards Player
	{
		/*float distanceToPLayer = Vector2Distance(level->tank.getPosition(), level->tA.getPosition());*/
		
		Vector2 pos = level->tank.getPosition();
		Vector2 targetPos = level->player.getPosition();

		//rotate towards player
		//Calulate angle
		Vector2 diff = Vector2Subtract(pos, targetPos);
		float angle = -atan2f(diff.x, diff.y) * RAD2DEG;
		level->tank.setRotation(angle);


		//move towards player
		if (pos.x < targetPos.x)
		{
			pos.x += level->tank.moveSpeed * GetFrameTime();
		}
		if (pos.x > targetPos.x)
		{
			pos.x -= level->tank.moveSpeed * GetFrameTime();
		}
		if (pos.y < targetPos.y)
		{
			pos.y += level->tank.moveSpeed * GetFrameTime();
		}
		if (pos.y > targetPos.y)
		{
			pos.y -= level->tank.moveSpeed * GetFrameTime();
		}

		level->tank.setPosition(pos);
		return true;
	}

	// HEALER BT ACTIONS

	// MONSTER BT ACTIONS

	return false;
}