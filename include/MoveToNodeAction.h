#pragma once

#include "GoapAction.h"

class MoveToNodeAction : public GoapAction {
public:
	MoveToNodeAction() : GoapAction(GoapAction::Type::MoveToNode, true, 3.f)
	{
		addEffect("reachedNode", true);
	}
	void reset(Unit* unit) override
	{

	}
	bool isDone(Unit* unit) override
	{
		return unit->arrivedAtTarget();
	}
	bool checkRunTimePreconditions(Unit* unit) override
	{
		return unit->getEnemiesInRange(unit->getRange() * 2).empty() == false; //still enemies in range
	}
	bool perform(Unit* unit) override
	{
		return true;
	}
};
