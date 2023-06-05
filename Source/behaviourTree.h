#pragma once

#include <vector>

#include <string>

class Level;

class BehaviourTree
{
public:

	BehaviourTree();

	//enum TankActions
	//{
	//	tank_checkOwnHealth,
	//	tank_checkAllyHealth,
	//	tank_moveTankTowardsHealer,
	//	tank_moveTankTowardsPlayer,
	//	tank_moveTankTowardsMonster,
	//	tank_attack,

	//} tankAction = tank_checkOwnHealth;


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

//class MonsterBT : public BehaviourTree
//{
//
//};

//class TankBT : public BehaviourTree
//{
//public:
//
//	TankBT();
//
//	//BehaviourTree tBT;
//
//	//TankBT::Selector selector[2];
//	//TankBT::Selector selector1 = selector[0];
//	//Action moveTowardsHealer = Action("moveTowardsHealer", 100);
//	//Action moveTowardsPlayer = Action("moveTowardsPlayer", 100);
//	//Action checkOwnHealth = Action("tankCheckOwnHealth", 100);
//
//	//const TankBT::Selector &GetRoot();
//	void setRootChild(Selector* rootChild);
//
//	Root* tBT_root;
//};

//class HealerBT : public BehaviourTree
//{
//
//};