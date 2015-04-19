#ifndef PLAYER_H
#define PLAYER_H

#include <entities/entity.h>
#include <resources/animation.h>
#include <array>
#include <vector>

#include <iostream>

class World;
class PlayerController;

class Player : public Entity
{
public:
    friend class PlayerController;  // Dirty

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
    void        fire(std::vector<sf::Vector2f>& path);
    void        retrieve();
    void        setCanMove(bool b);

    virtual void knock();
    virtual void unknock();
    virtual void move(Direction dir);

    bool        canFire() { return !mFired; }

    virtual bool isCollidable() const { return true; }
    virtual int  getAttackPower() const { return 0; }
    virtual void setDirection(Direction dir);

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const { return Category::Player; }

    virtual bool isMarkedForRemoval() const { return false; }
    //virtual bool isDestroyed();


protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    void         updateAnimation(sf::Time dt);

private:
    TextureHolder& mTextures;

    sf::Sprite  mStandingWithSprite;
    sf::Sprite  mStandingWithoutSprite;
    std::array<Animation, 4> mAnimations;
    sf::Sprite  mDeadWithoutSprite;
    sf::Sprite  mDeadWithSprite;
    bool        mMovingAnimation;

    // Attacking state
    bool        mCanMove;
    bool        mIsFiring;
    bool        mFired;
    std::vector<sf::Vector2f> mFirePath;
};

#endif // PLAYER_H
