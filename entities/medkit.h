#ifndef MEDKIT_H
#define MEDKIT_H

#include <entities/entity.h>

class Medkit : public Entity
{
public:
    Medkit(TextureHolder& textures);

    void    use(Entity& on);

    virtual unsigned int    getCategory() const { return Category::Medkit; }
    virtual bool            isCollidable() const { return !mUsed; }
    virtual bool            isDestroyed() const {return mUsed; }
    virtual sf::FloatRect   getBoundingRect() const
    {
        return getWorldTransform().transformRect(mSprite.getGlobalBounds());
    }

protected:
    virtual void    drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(mSprite, states);
    }

private:
    bool        mUsed;
    sf::Sprite  mSprite;
};

#endif // MEDKIT_H
