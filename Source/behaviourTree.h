#pragma once

#include <vector>

#include <string>

class Level;

class BehaviourTree
{
public:

	BehaviourTree();

	struct Node
	{
		virtual bool run(Level* level) = 0;
	};

	struct CompositeNode : public Node
	{
	private:

	public:
		std::vector<Node*> children;

		const std::vector<Node*>& getChildren();
		void addChild(Node* child);
		void addChildren(std::initializer_list<Node*>&& newChildren);
	};

	struct Selector : public CompositeNode
	{
	public:
		bool run(Level* level) override;
	};

	struct Sequence : public CompositeNode
	{
	public:
		bool run(Level* level) override;
	};

	struct DecoratorConditional : public CompositeNode
	{
		bool condition = false;
		bool run(Level* level) override;
		void setCondition(bool p_condition);
	};

	struct DecoratorSelector : public CompositeNode
	{
		bool condition = false;
		bool run(Level* level) override;
		void setCondition(bool p_condition);
	};

	struct DecoratorSequence : public CompositeNode
	{
		bool condition = false;
		bool run(Level* level) override;

		void setCondition(bool p_condition);
	};

	struct DecoratorAction : public BehaviourTree::Node
	{
		DecoratorAction(std::string newName, int prob);
		DecoratorAction(int p_actionId);
		bool condition = false;
		int actionId = -1;
		bool run(Level* level) override;
		std::string name;
		int probabilityOfSuccess;
		void setCondition(bool p_condition);
	};

	struct Root : public Node
	{
	public:
		Node* child = nullptr;
		//friend class BehaviourTree;
		void setChild(Selector* newChild);
		virtual bool run(Level* level) override;
	};

	void setRootChild(Selector* rootChild);
	bool run(Level* level) const { return root->run(level); }

private:

	Root* root;

};

struct Action : public BehaviourTree::Node
{
	Action(std::string newName, int prob);
	Action(int p_actionId);
	int actionId = -1;
	std::string name;
	int probabilityOfSuccess;
	bool run(Level* level) override; // pass in agent as argument

};