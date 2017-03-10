#include "AbductorStates.h"

void AFlockState::start(Abductor* abductor)
{
	abductor->setMoving(true);
}

void AFlockState::update(Abductor* abductor, float dt)
{
	int neighbourCount = abductor->getNeighbourCount();
	if (neighbourCount == 0)
	{
		abductor->changeState(ADropState::getInstance());
	}
	sf::Vector2f sep = abductor->separation();
	sf::Vector2f ali = abductor->alignment();
	sf::Vector2f coh = abductor->cohesion();
	sf::Vector2f acceleration;
	acceleration += sep;
	acceleration += ali;
	acceleration += coh;
	abductor->setAcceleration(acceleration);
	abductor->move(dt);
	abductor->checkWorldBounds();
	abductor->fire(dt);
	if (abductor->getAbducting())
	{
		abductor->changeState(AAbductingState::getInstance());
	}
}

void AFlockState::end(Abductor* abductor)
{
}

//-----------------------------------------------------------------------------
void ADropState::start(Abductor* abductor)
{
	abductor->setMoving(true);
}

void ADropState::update(Abductor* abductor, float dt)
{ 
	abductor->updateDropAcceleration();
	abductor->move(dt);
	if (abductor->reachedTarget())
	{
		abductor->changeState(APatrolState::getInstance());
	}
}

void ADropState::end(Abductor * abductor)
{
}

//-----------------------------------------------------------------------------
void APatrolState::start(Abductor* abductor)
{
	abductor->setMoving(true);
	abductor->setDirection(sf::Vector2f((abductor->getDirection().x > 0.f) ? 1.f : -1.f, 0.f));
}

void APatrolState::update(Abductor* abductor, float dt)
{ 
	int neighbourCount = abductor->getNeighbourCount();
	if (neighbourCount > 0)
	{
		abductor->changeState(AFlockState::getInstance());
	}
	abductor->updatePatrolAcceleration();
	abductor->checkWorldBounds();
	if (abductor->getAbducting())
	{
		abductor->changeState(AAbductingState::getInstance());
	}
}

void APatrolState::end(Abductor * abductor)
{
}


//-----------------------------------------------------------------------------
void AAbductingState::start(Abductor* abductor)
{
	abductor->setMoving(true);
	abductor->setAcceleration(sf::Vector2f());
}

void AAbductingState::update(Abductor* abductor, float dt)
{
	abductor->updateAbduction(dt);
	abductor->move(dt);
	abductor->checkAbductionBounds();
}

void AAbductingState::end(Abductor * abductor)
{
}
