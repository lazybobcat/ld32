#ifndef UNICORN_H
#define UNICORN_H

#include <entities/entity.h>
#include <entities/emitternode.h>
#include <vector>

class Unicorn : public Entity
{
public:
    Unicorn(std::vector<sf::Vector2f>& path, TextureHolder& textures);

    void         retrieve();
    bool         isTraveling() const { return mIsTraveling; }

    virtual bool isCollidable() const;
    virtual bool isDestroyed() const;
    virtual int getAttackPower() const { return 10; }

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const { return Category::Unicorn; }

protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite  mSprite;
    bool        mIsRetrieved;

    // Path and movement
    std::vector<sf::Vector2f>   mPath;
    bool                        mIsTraveling;
    float                       mSpeed;
    float                       mTravelTime;
    sf::Time                    mTimeSinceLastPoint;

    EmitterNode*                mEmitter;
};

#endif // UNICORN_H
