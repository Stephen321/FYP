#pragma once

#include "GoapAction.h"

class ReloadAction : public GoapAction {
public:
	ReloadAction() : GoapAction(GoapAction::Type::Reload, false, 0.5f)
	{//play reload animation, reload every time you fire
		addPrecondition("fired", true);
		addEffect("attack", true);
	}
	void reset(Unit* unit) override
	{
	}
	bool isDone(Unit* unit) override
	{
		return true;
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
