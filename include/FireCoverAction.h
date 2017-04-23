#pragma once

#include "GoapAction.h"

class FireCoverAction : public GoapAction {
public:
	FireCoverAction() : GoapAction(GoapAction::Type::FireCover, false, 2.f)
	{
		addPrecondition("inCover", true);
		addPrecondition("hasAmmo", true);
		addEffect("fired", true);
	}
	void reset(Unit* unit) override
	{
		m_done = false;
	}
	bool isDone(Unit* unit) override
	{
		return m_done;
	}
	bool checkRunTimePreconditions(Unit* unit) override
	{
		return true; 
	}
	bool perform(Unit* unit) override
	{
		unit->fireAtTarget(true);
		m_done = true;
		return true;
	}
private:
	bool m_done;
};
