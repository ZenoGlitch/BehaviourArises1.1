#pragma once

#include "agent.h"
#include <vector>

#include <string>

class Level;

enum NodeState
{
	running,
	success,
	failure
};

class BehaviourTree
{
public:

	BehaviourTree();

	struct Node
	{
		virtual bool run(Level* level, Agent *agent) = 0;
		NodeState state = failure;
	};

	struct CompositeNode : public Node
	{
	public:
		std::vector<Node*> children;

		const std::vector<Node*>& getChildren();
		void addChild(Node* child);
		void addChildren(std::initializer_list<Node*>&& newChildren);
	};

	struct Selector : public CompositeNode
	{
	public:
		bool run(Level* level, Agent* agent) override;
	};

	struct Sequence : public CompositeNode
	{
	public:
		bool run(Level* level, Agent* agent) override;
	};

	struct DecoratorConditional : public CompositeNode
	{
		bool condition = false;
		bool run(Level* level, Agent* agent) override;
		void setCondition(bool p_condition);
	};

	struct DecoratorSelector : public CompositeNode
	{
		bool condition = false;
		bool run(Level* level, Agent* agent) override;
		void setCondition(bool p_condition);
	};

	struct DecoratorSequence : public CompositeNode
	{
		bool condition = false;
		bool run(Level* level, Agent* agent) override;

		void setCondition(bool p_condition);
	};

	struct DecoratorAction : public BehaviourTree::Node
	{
		DecoratorAction(int p_actionId);
		bool condition = false;
		int actionId = -1;
		bool run(Level* level, Agent* agent) override;
		void setCondition(bool p_condition);
	};

	struct Action : public BehaviourTree::Node
	{
	public:
		Action(int p_actionId);
		int actionId = -1;
		bool run(Level* level, Agent *agent) override; 
	};

private:

};
