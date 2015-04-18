#ifndef AISTATEMACHINE_H
#define AISTATEMACHINE_H

#include <controllers/AIStates/aistate.h>
#include <SFML/System/Time.hpp>
#include <vector>

class Player;

class AIStateMachine
{
public:
    enum States
    {
        Moving,
        Attacking,
        Waiting
    };

    enum Action
    {
        Push,
        Pop,
        Clear
    };

public:
    AIStateMachine();

/*    void        update(sf::Time dt);

    void        push(States state);
    void        pop();
    void        clear();
    bool        isEmpty() const;

    AIState*    getCurrentState();

private:
    void        applyPendingChanges();

private:
    struct PendingChange
    {
        explicit PendingChange(Action action, States state = Moving);
        Action      action;
        States      state;
    };

private:
    std::vector<AIState*>       mStates;
    std::vector<PendingChange>  mPendingList;
*/

};

#endif // AISTATEMACHINE_H
