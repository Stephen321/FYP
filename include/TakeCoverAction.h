#pragma once

#include "GoapAction.h"

class TakeCoverAction : public GoapAction {
public:
	TakeCoverAction() : GoapAction(GoapAction::Type::TakeCover, true, 8.f)
	{
		addEffect("hiddenCover", true);
		addEffect("healing", true);
	}
	void reset(Unit* unit) override
	{
		m_startPosition = unit->getPosition();
	}
	bool isDone(Unit* unit) override
	{
		return unit->inCover() && unit->getPosition() != m_startPosition;// && unit->getEnemiesInRange().empty();
	}
	bool checkRunTimePreconditions(Unit* unit) override
	{
		return true;
	}
	bool perform(Unit* unit) override
	{
		m_startPosition = unit->getPosition();
		return true;
	}
private:
	sf::Vector2f m_startPosition;
};
