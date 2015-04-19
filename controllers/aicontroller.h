#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <events/commandqueue.h>
#include <entities/zombie.h>
#include <entities/player.h>
#include <controllers/AIStates/movingbehavior.h>
#include <controllers/AIStates/attackingbehavior.h>
#include <controllers/AIStates/waitingbehavior.h>

class AIController
{
public:
    AIController();

    virtual void    update(sf::Time dt, CommandQueue& commands, Zombie& zombie);

private:
    Player*     mPlayer;
};

#endif // AICONTROLLER_H
