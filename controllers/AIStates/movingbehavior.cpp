#include "movingbehavior.h"
#include <controllers/AIStates/attackingbehavior.h>
#include <controllers/AIStates/waitingbehavior.h>

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

    if(dist > 40)
    {
        if(direction.x > 0)
        {
            entity.move(Entity::Right);
        }
        else
        {
            entity.move(Entity::Left);
        }

        if(std::abs(direction.x) < 40)
        {
            if(dist > 150 && dist < 250)  entity.jump();
            else            return new WaitingBehavior(mPlayer, 0.5f);
        }
    }
    else
    {
        return new AttackingBehavior(mPlayer);
    }

    return nullptr;
}
