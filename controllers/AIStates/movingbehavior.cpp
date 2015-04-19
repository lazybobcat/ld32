#include "movingbehavior.h"
#include <controllers/AIStates/attackingbehavior.h>
#include <controllers/AIStates/waitingbehavior.h>

#include <iostream>

MovingBehavior::MovingBehavior(Player &player) :
    AIState(player)
{
}

void MovingBehavior::init(Zombie &/*entity*/)
{
}

void MovingBehavior::deinit(Zombie &/*entity*/)
{
}


AIState* MovingBehavior::run(Zombie &entity, sf::Time dt, CommandQueue &commands)
{
    mTimer += dt;

    sf::Vector2f direction (mPlayer.getPosition() - entity.getPosition());
    float        dist = distance(mPlayer.getPosition(), entity.getPosition());

    if(mPlayer.isDestroyed())
    {
        return new WaitingBehavior(mPlayer, 10.f);
    }

    if(entity.isResting())
    {
        return new WaitingBehavior(mPlayer, 0.85f);
    }

    Entity::Direction dir = Entity::Right;

    if(direction.y > 10) // Zombie above
    {
        if(entity.getPosition().x < 1015.f)
            dir = Entity::Right;
        else
            dir = Entity::Left;
    }
    else if(direction.y < -10) // Zombie below
    {
        if(std::abs(direction.x) < 40 && dist > 150 && dist < 250) // If we can jump
        {
            entity.jump();
        }
        else if(std::abs(direction.x) < 40 && dist > 250) // Too far to jump
        {
            return new WaitingBehavior(mPlayer, 0.5f);
        }
        else
        {
            if(direction.x > 0)
            {
                dir = Entity::Right;
            }
            else
            {
                dir = Entity::Left;
            }
        }
    }
    else if(dist > 40) // Same level far
    {
        if(direction.x > 0)
        {
            dir = Entity::Right;
        }
        else
        {
            dir = Entity::Left;
        }
    }
    else
    {
        return new AttackingBehavior(mPlayer);
    }

    entity.move(dir);

    return nullptr;
}
