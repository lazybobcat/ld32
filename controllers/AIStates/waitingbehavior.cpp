#include "waitingbehavior.h"
#include <controllers/AIStates/movingbehavior.h>

WaitingBehavior::WaitingBehavior(Player &player, float seconds) :
    AIState(player)
{
    mWait = sf::seconds(seconds);
}

void WaitingBehavior::init(Creature &entity)
{
    entity.stop();
}

void WaitingBehavior::deinit(Creature &/*entity*/)
{
}


AIState* WaitingBehavior::run(Creature &entity, sf::Time dt, CommandQueue &commands)
{
    mTimer += dt;

    if(mTimer >= mWait)
    {
        return new MovingBehavior(mPlayer);
    }

    return nullptr;
}
