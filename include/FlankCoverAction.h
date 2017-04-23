#pragma once

#include "GoapAction.h"

class FlankCoverAction : public GoapAction {
public:
	FlankCoverAction() : GoapAction(GoapAction::Type::FlankCover, true, 12.f)
	{
		addEffect("flankCover", true);
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
