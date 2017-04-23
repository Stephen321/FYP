#pragma once

#include <vector>


class GoapAction;

class GoapPlanner
{
public:
	typedef std::unordered_map<std::string, bool> State;
	struct Node {
		Node* parent;
		float runningCost;
		State state;
		GoapAction* action;

		Node(Node* _parent, float _runningCost, State _state, GoapAction* _action)
			: parent(_parent)
			, runningCost(_runningCost)
			, state(_state)
			, action(_action) {};
	};

	//get actions needed to complete goal
	std::vector<GoapAction*> plan(Unit* unit, std::vector<GoapAction*> availableActions, State worldState, State goal)

	{
		if (goal.empty())
		{
			//no plan
			return std::vector<GoapAction*>();
		}

		for (GoapAction* a : availableActions)
		{
			a->reset(unit);
		}

		std::vector<GoapAction*> usableActions = std::vector<GoapAction*>();
		for (GoapAction* a : availableActions)
		{
			if (a->checkRunTimePreconditions(unit))
				usableActions.push_back(a);
		}
		//leavesa will contain nodes that have the goal state
		std::vector<Node*> leaves;

		// build graph
		Node* start = new Node(nullptr, -1, worldState, nullptr);
		bool success = buildTree(start, leaves, usableActions, goal);

		if (success == false)
			return std::vector<GoapAction*>();

		Node* cheapest = nullptr;
		for (Node* leaf : leaves)
		{
			if (cheapest == nullptr)
				cheapest = leaf;
			else
			{
				if (leaf->runningCost < cheapest->runningCost)
					cheapest = leaf;
			}
		}

		std::vector<GoapAction*> result = std::vector<GoapAction*>();
		for (Node* n = cheapest; n != nullptr; n = n->parent)
		{
			if (n->action != nullptr)
				result.push_back(n->action);
		}

		for (int i = 0; i < leaves.size(); i++)
		{
			delete leaves[i];
			leaves[i] = nullptr;
		}

		return result;
	}

private:
	bool buildTree(Node* parent, std::vector<Node*>& leaves, std::vector<GoapAction*> usableActions, State goal)
	{
		bool foundOne = false;

		// go through each action available at this node and see if we can use it here
		for (GoapAction* action : usableActions)
		{
			// if the parent state has the conditions for this action's preconditions, we can use it here
			if (checkInState(action->getPreconditions(), parent->state))
			{

				// apply the action's effects to the parent state
				State currentState = parent->state;
				applyState(currentState, action->getEffects());
				Node* node = new Node(parent, parent->runningCost + action->getCost(), currentState, action);

				if (checkInState(goal, currentState))
				{
					// in goal state 
					leaves.push_back(node);
					foundOne = true;
				}
				else
				{
					//try more combinations of actions
					std::vector<GoapAction*> remainingActions = removeAction(usableActions, action);
					bool found = buildTree(node, leaves, remainingActions, goal);
					if (found)
						foundOne = true;
				}
			}
		}

		return foundOne;
	}

	std::vector<GoapAction*> removeAction(std::vector<GoapAction*> actions, GoapAction* action)
	{
		std::vector<GoapAction*> subset = std::vector<GoapAction*>();
		for (GoapAction* a : actions) {
			if (a != action)
				subset.push_back(a);
		}
		return subset;
	}

	bool checkInState(const State& s, const State& state)
	{
		bool allMatch = true;
		for (std::pair<std::string, bool> t : s)
		{
			bool match = false;
			for (std::pair<std::string, bool> s : state)
			{
				if (s == t)
				{
					match = true;
					break;
				}
			}
			if (!match)
				allMatch = false;
		}
		return allMatch;
	}
	void applyState(State& state, const State& change)
	{

		for (std::pair<std::string, bool> change : change) {
			if (state.find(change.first) != state.end())
			{
				state.at(change.first) = change.second;
			}
			// if it does not exist in the current state, add it
			else {
				state.insert(std::pair<std::string, bool>(change.first, change.second));
			}
		}
	}

};

#include "GoapAction.h"

