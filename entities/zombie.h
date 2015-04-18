#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <entities/entity.h>
#include <resources/animation.h>
#include <array>

class Zombie : public Entity
{
public:
    enum Animations
    {
        Walking,
        Attacking,
        Dying
    };

public:
    Zombie(TextureHolder& textures);

    void        attack();

    virtual bool isCollidable() const { return true; }
    virtual int  getAttackPower() const { return 20; }
    virtual void setDirection(Direction dir);

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const { return Category::Enemy; }

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    void         updateAnimation(sf::Time dt);

private:
    std::array<Animation, 3> mAnimations;
    bool                     mIsAttacking;
    bool                     mMovingAnimation;
};

#endif // ZOMBIE_H
