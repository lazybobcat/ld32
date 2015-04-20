#include "unicorn.h"
#include <utils.h>

#include <iostream>

Unicorn::Unicorn(std::vector<sf::Vector2f> &path, TextureHolder &textures) :
    Entity(1),
    mIsRetrieved(false),
    mPath(path),
    mIsTraveling(true),
    mSpeed(200.f),
    mTravelTime(0.07f),
    mTimeSinceLastPoint(sf::Time::Zero),
    mEmitter(nullptr)
{
    mSprite.setTexture(textures.get(Textures::Hero));
    mSprite.setTextureRect(sf::IntRect(300, 0, 75, 75));

    setOrigin(sf::Vector2f(35.f, 55.f));

    if(mPath.size() >= 2)
    {
        setPosition(mPath[0]);
        mPath.erase(mPath.begin());
        mSpeed = distance(mPath[0], mPath[1]) / mTravelTime;
    }
    else
    {
        mIsTraveling = false;
    }

    std::unique_ptr<EmitterNode> particles(new EmitterNode(Particle::Default, EmitterNode::Rainbow));
    particles->setPosition(sf::Vector2f(37,60));
    mEmitter = particles.get();
    attachChild(std::move(particles));
}

void Unicorn::retrieve()
{
    if(!mIsTraveling)
        mIsRetrieved = true;
}

bool Unicorn::isCollidable() const
{
    return mIsTraveling;
}

bool Unicorn::isDestroyed() const
{
    return !mIsTraveling && mIsRetrieved;
}

sf::FloatRect Unicorn::getBoundingRect() const
{
    return getWorldTransform().transformRect(sf::FloatRect(10.f, 20.f, 50.f, 50.f));
}


void Unicorn::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(mIsTraveling)
    {
        mTimeSinceLastPoint += dt;
        sf::Vector2f diff = mPath.front() - getPosition();
        sf::Vector2f direction = unitVector(diff);
        if(diff.x < 0)  setScale(-1.f, 1.f);
        else            setScale(1.f, 1.f);
        float delta = mSpeed * dt.asSeconds();
        sf::Vector2f mov(direction.x*delta, direction.y*delta);
        sf::Transformable::move(mov);

        if(getPosition() == mPath.front() || mTimeSinceLastPoint.asSeconds() >= mTravelTime)
        {
            sf::Vector2f currentPosition = mPath.front();
            setPosition(currentPosition);
            mTimeSinceLastPoint = sf::Time::Zero;
            mPath.erase(mPath.begin());
            if(mPath.empty())
            {
                mIsTraveling = false;
                if(currentPosition.y > 768)
                {
                    currentPosition.y = 680;
                    setPosition(currentPosition);
                }
                if(mEmitter) detachChild(*mEmitter);
                mEmitter = nullptr;
            }
            else
            {
                mSpeed = distance(currentPosition, mPath.front()) / mTravelTime;
            }
        }
    }
    else
    {
        Entity::updateCurrent(dt, commands);
    }
}

void Unicorn::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(isDestroyed()) return;

    target.draw(mSprite, states);

    /*sf::FloatRect bound(10.f, 20.f, 50.f, 50.f);
    sf::RectangleShape shape(sf::Vector2f(bound.width, bound.height));
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color(255,255,255, 0));
    shape.setPosition(bound.left, bound.top);
    target.draw(shape, states);*/
}
