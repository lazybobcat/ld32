#include "waitingbehavior.h"
#include <controllers/AIStates/movingbehavior.h>

WaitingBehavior::WaitingBehavior(Player &player, float seconds) :
    AIState(player)
{
    mWait = sf::seconds(seconds);
}

void WaitingBehavior::init(Zombie &entity)
{
    entity.stop();
}

void WaitingBehavior::deinit(Zombie &/*entity*/)
{
}


AIState* WaitingBehavior::run(Zombie &entity, sf::Time dt, CommandQueue &commands)
{
    mTimer += dt;

    if(mTimer >= mWait)
    {
        return new MovingBehavior(mPlayer);
    }

    return nullptr;
}
