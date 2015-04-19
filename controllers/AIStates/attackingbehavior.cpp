#include "attackingbehavior.h"
#include <controllers/AIStates/movingbehavior.h>
#include <controllers/AIStates/waitingbehavior.h>

AttackingBehavior::AttackingBehavior(Player &player) :
    AIState(player)
{
}

void AttackingBehavior::init(Creature &/*entity*/)
{
}

void AttackingBehavior::deinit(Creature &/*entity*/)
{
}

AIState* AttackingBehavior::run(Creature &entity, sf::Time dt, CommandQueue &commands)
{
    assert(dynamic_cast<Zombie*>(&entity) != nullptr);
    Zombie& z = static_cast<Zombie&>(entity);

    return run(z, dt, commands);
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
