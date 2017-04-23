#pragma once

#include "GoapAction.h"

class IdleAction: public GoapAction {
public:
	IdleAction() : GoapAction(GoapAction::Type::Idle, false, 1.f)
	{
		addEffect("isIdle", true);
	}
	void reset(Unit* unit) override
	{

	}
	bool isDone(Unit* unit) override
	{
		return true; //animation is idle image
	}
	bool checkRunTimePreconditions(Unit* unit) override
	{
		return true;
	}
	bool perform(Unit* unit) override
	{
		return true;
	}
};
