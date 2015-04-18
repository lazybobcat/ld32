#ifndef PLAYER_H
#define PLAYER_H

#include <entities/entity.h>
#include <resources/animation.h>
#include <array>

class World;
class PlayerController;

class Player : public Entity
{
public:
    friend class World;  // Boooooo that's so dirtyyyyy...
    friend class PlayerController;  // Less dirty, but still ...

    enum Animations
    {
        WalkingWithout,
        WalkingWith,
        JumpingWithout,
        JumpingWith,
    };

public:
    Player(TextureHolder& textures);

    void        jump();

    virtual bool isCollidable() const { return true; }
    virtual int getAttackPower() const { return 0; }

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const { return Category::Player; }


protected:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite  mStandingSprite;
    std::array<Animation, 4> mAnimations;
    sf::Sprite  mDeadWithoutSprite;
    sf::Sprite  mDeadWithSprite;

    // Physic state
    bool        mIsJumping;
    float       mVerticalVelocity;

    // Attacking state
    bool        mFired;
};

#endif // PLAYER_H
