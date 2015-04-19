#ifndef CREATURE_H
#define CREATURE_H

#include <entities/entity.h>
#include <resources/animation.h>
#include <array>

class AIState;
class AttackingBehavior;
class AIController;

class Creature : public Entity
{
public:
    friend class AttackingBehavior;
    friend class AIController;

public:
    Creature(AIController& controller, int hp);
    virtual ~Creature();

    virtual void        jump();
    virtual void        attack();
    virtual void        die();
    virtual void        damage(int points);
    virtual void        knock();
    virtual void        unknock();
    bool                isResting() const;
    virtual void        rest();
    virtual void        unrest();
    virtual bool        isAttacking() const { return mIsAttacking; }

    virtual unsigned int    getCategory() const { return Category::Enemy; }

    void        setBehavior(AIState* behavior);

    virtual bool isCollidable() const { return !mIsDying; }

protected:
    bool                     mIsResting;
    sf::Time                 mRestingTime;
    bool                     mIsAttacking;
    bool                     mIsDying;
    sf::Time                 mDepopTimer;

    // States / Behaviors
    AIController&           mAIController;
    AIState*                mCurrentBehavior;
};

#endif // CREATURE_H
