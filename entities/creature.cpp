#include "creature.h"
#include <controllers/aicontroller.h>
#include <controllers/AIStates/aistate.h>

Creature::Creature(AIController& controller, int hp) :
    Entity(hp),
    mIsResting(false),
    mIsAttacking(false),
    mIsDying(false),
    mAIController(controller),
    mCurrentBehavior(nullptr)
{
}

Creature::~Creature()
{
    if(mCurrentBehavior)
    {
        mCurrentBehavior->deinit(*this);
        delete(mCurrentBehavior);
    }
}

void Creature::jump()
{
    if(!mIsJumping)
    {
        sf::Vector2f pos = getPosition();
        pos.y -= 2.f;
        setPosition(pos); // Hack to avoid unwated collisions with the platform
        mVerticalVelocity = -450;
        mIsJumping = true;
        rest();
    }
}

void Creature::attack()
{
    mIsAttacking = true;
}

void Creature::damage(int points)
{
    Entity::damage(points);

    if(mHealthpoints <= 0) die();
}

void Creature::knock()
{
    Entity::knock();
    stop();
    //mIsAttacking = false;
}

void Creature::unknock()
{
    Entity::unknock();
}

bool Creature::isResting() const
{
    return mIsResting;
}

void Creature::rest()
{
    if(!mIsResting)
    {
        mIsResting = true;
        mRestingTime = sf::Time::Zero;
    }
}

void Creature::unrest()
{
    mIsResting = false;
    mRestingTime = sf::Time::Zero;
}

void Creature::die()
{
    mIsDying = true;
}


void Creature::setBehavior(AIState *behavior)
{
    if(mCurrentBehavior)
    {
        mCurrentBehavior->deinit(*this);
        delete(mCurrentBehavior);
        mCurrentBehavior = nullptr;
    }

    if(behavior)
    {
        mCurrentBehavior = behavior;
        mCurrentBehavior->init(*this);
    }
}
