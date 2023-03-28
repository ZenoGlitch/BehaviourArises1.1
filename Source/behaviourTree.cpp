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
	if (name == "testing1")
	{
		printf("test 1 succeeded\n");
		return true;
	}

	if (name == "moveTowardsPlayer")
	{
		float distanceToPLayer = Vector2Distance(level->tank.getPosition(), level->tA.getPosition());
		//rotate towards player



		//move towards player
	}
	return false;
}