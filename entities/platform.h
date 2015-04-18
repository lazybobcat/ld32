#ifndef PLATFORM_H
#define PLATFORM_H

#include <entities/entity.h>

class Platform : public Entity
{
public:
    enum Type
    {
        Small,
        Medium,
        Ground
    };

public:
    Platform(Platform::Type type, TextureHolder& textures);

    virtual unsigned int    getCategory() const { return Category::Platform; }
    virtual bool            isDestroyed() const { return false; }
    virtual bool            isCollidable() const { return true; }
    virtual sf::FloatRect   getBoundingRect() const;

protected:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite  mSprite;
};

#endif // PLATFORM_H
