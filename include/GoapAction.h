#pragma once

#include <unordered_map>
#include <queue>

class Unit;
class GoapAction {
public:
	typedef std::unordered_map<std::string, bool> State;

	enum class Type {
		Fire,
		Reload,
		Idle,
		TakeCover,
		PickUpAmmo,
		FireCover,
		MoveToNode,
		FlankCover
	};

	GoapAction(Type type, bool needsInRange, float cost);

	virtual void reset(Unit* unit) {};
	virtual bool isDone(Unit* unit) = 0;
	virtual bool checkRunTimePreconditions(Unit* unit) = 0;
	virtual bool perform(Unit* unit) = 0;

	bool getInRange() 
	{
		return m_inRange;
	}

	void setInRange(bool inRange) 
	{
		m_inRange = inRange;
	}

	void addPrecondition(std::string key, bool value);
	void addEffect(std::string key, bool value);

	State getPreconditions()
	{
		return m_preconditions;
	}

	State getEffects()
	{
		return m_effects;
	}

	Type getType()
	{
		return m_type;
	}

	bool getNeedsInRange() 
	{
		return m_needsInRange;
	}

	float getCost() const
	{
		return m_cost;
	}

private:
	float m_cost = 1.f;
	bool m_needsInRange = false;
	State m_preconditions;
	State m_effects;
	Type m_type;
	bool m_inRange = false;
};
#include "Unit.h"