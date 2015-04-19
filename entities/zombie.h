#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <entities/entity.h>
#include <resources/animation.h>
#include <array>

class AIState;
class AttackingBehavior;
class AIController;

class Zombie : public Entity
{
public:
    friend class AttackingBehavior;
    friend class AIController;

public:
    enum Animations
    {
        Walking,
        Attacking,
        Dying
    };

public:
    Zombie(TextureHolder& textures, AIController& controller);
    virtual ~Zombie();

    void        jump();
    void        attack();
    void        die();
    void        damage(int points);
    void        knock();
    void        unknock();
    bool        isResting() const;
    void        rest();
    void        unrest();


    void        setBehavior(AIState* behavior);

    virtual bool isCollidable() const { return !mIsDying; }
    virtual bool isDestroyed() const;
    virtual int  getAttackPower() const { return 20; }
    virtual void setDirection(Direction dir);
    bool         isAttacking() const { return mIsAttacking; }

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const { return Category::Enemy; }

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    void         updateAnimation(sf::Time dt);

private:
    sf::Sprite               mWaitingSprite;
    sf::Sprite               mDeadSprite;
    std::array<Animation, 3> mAnimations;
    bool                     mIsResting;
    sf::Time                 mRestingTime;
    bool                     mIsAttacking;
    bool                     mMovingAnimation;
    bool                     mIsDying;
    sf::Time                 mDepopTimer;

    // States / Behaviors
    AIController&           mAIController;
    AIState*                mCurrentBehavior;
};

#endif // ZOMBIE_H
