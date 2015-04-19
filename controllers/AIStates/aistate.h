#ifndef AISTATE_H
#define AISTATE_H

#include <events/commandqueue.h>

class Creature;
class Player;

class AIState
{
public:
    AIState(Player& player);
    virtual ~AIState() {}

    virtual AIState*    run(Creature& entity, sf::Time dt, CommandQueue& commands) = 0;
    virtual void        init(Creature& entity) = 0;
    virtual void        deinit(Creature& entity) = 0;

protected:
    Player&         mPlayer;
};

#endif // AISTATE_H
