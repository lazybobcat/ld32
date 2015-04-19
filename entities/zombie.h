#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <entities/creature.h>
#include <entities/emitternode.h>
#include <resources/animation.h>
#include <array>

class AIState;
class AttackingBehavior;
class AIController;

class Zombie : public Creature
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

    virtual void        attack();
    virtual void        knock();
    virtual void        unknock();

    virtual bool isDestroyed() const;
    virtual int  getAttackPower() const { return 20; }
    virtual void setDirection(Direction dir);

    virtual sf::FloatRect   getBoundingRect() const;

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    void         updateAnimation(sf::Time dt);

private:
    sf::Sprite               mWaitingSprite;
    sf::Sprite               mDeadSprite;
    std::array<Animation, 3> mAnimations;
    bool                     mMovingAnimation;

    EmitterNode*                mEmitter;
};

#endif // ZOMBIE_H
