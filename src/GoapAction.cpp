#include "GoapAction.h"

GoapAction::GoapAction(Type type, bool needsInRange, float cost)
	: m_type(type)
	, m_needsInRange(needsInRange)
	, m_cost(cost)
{
	m_preconditions = State();
	m_effects = State();
}

void GoapAction::addPrecondition(std::string key, bool value)
{
	m_preconditions.insert(std::pair<std::string, bool>(key, value));
}

void GoapAction::addEffect(std::string key, bool value)
{
	m_effects.insert(std::pair<std::string, bool>(key, value));
}