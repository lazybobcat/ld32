#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Time.hpp>

#include <entities/scenenode.h>
#include <entities/soundnode.h>

class CommandQueue;

class Entity : public SceneNode
{
public:
    enum Direction
    {
        Right,
        Left
    };

public:
    Entity(int healthpoints = 1);

    void        damage(int points);
    void        heal(int points);
    void        destroy();
    int         getHealthpoints() const;
    virtual bool        isDestroyed() const;
    virtual int getAttackPower() const { return 0; }
    bool        isKnocked() const;

    Direction   getDirection() const;
    virtual void setDirection(Direction dir);

    virtual void move(Direction dir);
    void         demove(); // Soooooo ugly ....
    void         stop();
    bool         isMoving() const;
    void        knock();
    void        unknock();

    void        playLocalSound(CommandQueue& commands, Sounds::ID sound);

protected:
    virtual void    updateCurrent(sf::Time dt);

protected:
    float       mHorizontalVelocity;

private:
    int         mHealthpoints;
    bool        mJustGetHurt;
    sf::Time    mLastHurtTime;
    bool        mKnocked;

    Direction   mDirection;
    bool        mIsMoving;

    sf::Vector2f mLastSafePosition;

};

#endif // ENTITY_H
