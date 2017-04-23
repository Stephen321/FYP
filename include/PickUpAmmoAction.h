#pragma once

#include "GoapAction.h"

class PickUpAmmoAction : public GoapAction {
public:
	PickUpAmmoAction() : GoapAction(GoapAction::Type::PickUpAmmo, true, 3.f)
	{
		addEffect("hasAmmo", true);
	}
	void reset(Unit* unit) override
	{
		m_ammo = unit->getAmmo();
	}
	bool isDone(Unit* unit) override
	{
		return m_ammo < unit->getAmmo() || unit->ammoTargetExists() == false; //when unit has more ammo than it started action with or ammo no longer exists there
	}
	bool checkRunTimePreconditions(Unit* unit) override
	{
		return unit->ammoTargetExists();
	}
	bool perform(Unit* unit) override
	{
		return true;
	}
private:
	int m_ammo;
};
