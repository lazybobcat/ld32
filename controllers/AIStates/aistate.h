#ifndef AISTATE_H
#define AISTATE_H

#include <events/commandqueue.h>

class Zombie;
class Player;

class AIState
{
public:
    AIState(Player& player);
    virtual ~AIState() {}

    virtual AIState*    run(Zombie& entity, sf::Time dt, CommandQueue& commands) = 0;
    virtual void        init(Zombie& entity) = 0;
    virtual void        deinit(Zombie& entity) = 0;

protected:
    Player&         mPlayer;
};

#endif // AISTATE_H
