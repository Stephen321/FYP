#pragma once

#include "GoapAction.h"
#include "GoapPlanner.h"
#include <queue>
#include <memory>

class GoapAgent 
{
public:
	typedef std::unordered_map<std::string, bool> State;

	GoapAgent()
		:  m_state(FSMState::Idle)
	{};

protected:
	bool hasPlan() 
	{
		return m_currentActions.size() > 0;
	}

	enum class FSMState {
		Idle,
		MoveTo,
		PerformAction,
		None
	};

	FSMState m_state;

	std::vector<GoapAction*> m_availableActions;
	std::vector<GoapAction*> m_currentActions;

	GoapPlanner m_planner;
};