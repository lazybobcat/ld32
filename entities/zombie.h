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
        Dying
    };

public:
    Zombie(TextureHolder& textures);

    void        attack();

    virtual bool isCollidable() const { return true; }
    virtual int  getAttackPower() const { return 20; }
    virtual void setDirection(Direction dir);

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const { return Category::Player; }
};

#endif // ZOMBIE_H
