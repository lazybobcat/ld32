#ifndef MOVINGBEHAVIOR_H
#define MOVINGBEHAVIOR_H

#include <utils.h>
#include <controllers/AIStates/aistate.h>
#include <entities/zombie.h>
#include <entities/player.h>

class MovingBehavior : public AIState
{
public:
    MovingBehavior(Player& player);

    virtual AIState*    run(Zombie& entity, sf::Time dt, CommandQueue& commands);
    virtual void        init(Zombie& entity);
    virtual void        deinit(Zombie& entity);

private:
    sf::Time            mTimer;
};

#endif // MOVINGBEHAVIOR_H
