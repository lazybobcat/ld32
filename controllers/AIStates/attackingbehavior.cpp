#include "attackingbehavior.h"
#include <controllers/AIStates/movingbehavior.h>
#include <controllers/AIStates/waitingbehavior.h>

AttackingBehavior::AttackingBehavior(Player &player) :
    AIState(player)
{
}

void AttackingBehavior::init(Zombie &/*entity*/)
{
}

void AttackingBehavior::deinit(Zombie &/*entity*/)
{
}


AIState* AttackingBehavior::run(Zombie &entity, sf::Time dt, CommandQueue &commands)
{
    sf::Vector2f direction (mPlayer.getPosition() - entity.getPosition());
    float        dist = distance(mPlayer.getPosition(), entity.getPosition());


    // If already attacking
    if(entity.mIsAttacking)
    {
        if(entity.mAnimations[Zombie::Attacking].isFinished())
        {
            entity.mIsAttacking = false;
            entity.mAnimations[Zombie::Attacking].restart();
            return new WaitingBehavior(mPlayer, 1.f);
        }

        return nullptr;
    }


    if(dist <= 40)
    {
        if(direction.x > 0)
        {
            entity.setDirection(Entity::Right);
        }
        else
        {
            entity.setDirection(Entity::Left);
        }

        entity.attack();
    }
    else
    {
        return new MovingBehavior(mPlayer);
    }

    return nullptr;
}
