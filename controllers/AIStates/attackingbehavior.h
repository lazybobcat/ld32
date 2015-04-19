#ifndef ATTACKINGBEHAVIOR_H
#define ATTACKINGBEHAVIOR_H

#include <utils.h>
#include <controllers/AIStates/aistate.h>
#include <entities/zombie.h>
#include <entities/player.h>

class AttackingBehavior : public AIState
{
public:
    AttackingBehavior(Player& player);

    virtual AIState*    run(Zombie& entity, sf::Time dt, CommandQueue& commands);
    virtual void        init(Zombie& entity);
    virtual void        deinit(Zombie& entity);

private:
    sf::Time            mTimer;
};

#endif // ATTACKINGBEHAVIOR_H
