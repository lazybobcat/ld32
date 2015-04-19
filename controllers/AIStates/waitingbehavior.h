#ifndef WAITINGBEHAVIOR_H
#define WAITINGBEHAVIOR_H

#include <utils.h>
#include <controllers/AIStates/aistate.h>
#include <entities/zombie.h>
#include <entities/player.h>

class WaitingBehavior : public AIState
{
public:
    WaitingBehavior(Player& player, float seconds = 2.f);

    virtual AIState*    run(Zombie& entity, sf::Time dt, CommandQueue& commands);
    virtual void        init(Zombie& entity);
    virtual void        deinit(Zombie& entity);

private:
    sf::Time            mTimer;
    sf::Time            mWait;
};

#endif // WAITINGBEHAVIOR_H
