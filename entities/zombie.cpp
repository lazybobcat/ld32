#include "zombie.h"
#include <controllers/AIStates/aistate.h>

Zombie::Zombie(TextureHolder &textures) :
    Entity(10),
    mIsAttacking(false),
    mMovingAnimation(false),
    mCurrentBehavior(nullptr)
{
    textures.get(Textures::ZombieWalking).setSmooth(false);
    textures.get(Textures::ZombieAttacking).setSmooth(false);
    textures.get(Textures::ZombieDying).setSmooth(false);

    mAnimations[Walking].setTexture(textures.get(Textures::ZombieWalking));
    mAnimations[Walking].setFrameSize(sf::Vector2i(75,75));
    mAnimations[Walking].setNumFrames(4);
    mAnimations[Walking].setDuration(sf::seconds(0.9f));
    mAnimations[Walking].setRepeating(true);
    mAnimations[Walking].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Attacking].setTexture(textures.get(Textures::ZombieAttacking));
    mAnimations[Attacking].setFrameSize(sf::Vector2i(75,75));
    mAnimations[Attacking].setNumFrames(4);
    mAnimations[Attacking].setDuration(sf::seconds(0.4f));
    mAnimations[Attacking].setRepeating(false);
    mAnimations[Attacking].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Dying].setTexture(textures.get(Textures::ZombieDying));
    mAnimations[Dying].setFrameSize(sf::Vector2i(75,75));
    mAnimations[Dying].setNumFrames(4);
    mAnimations[Dying].setDuration(sf::seconds(0.6f));
    mAnimations[Dying].setRepeating(false);
    mAnimations[Dying].setScale(sf::Vector2f(2.f, 2.f));
}


Zombie::~Zombie()
{
    if(mCurrentBehavior)
    {
        mCurrentBehavior->deinit(*this);
        delete(mCurrentBehavior);
    }
}


void Zombie::attack()
{
    mIsAttacking = true;
    mAnimations[Attacking].restart();
}


void Zombie::setBehavior(AIState *behavior)
{
    if(mCurrentBehavior)
    {
        mCurrentBehavior->deinit(*this);
        delete(mCurrentBehavior);
        mCurrentBehavior = nullptr;
    }

    if(behavior)
    {
        mCurrentBehavior = behavior;
        mCurrentBehavior->init(*this);
    }
}


sf::FloatRect Zombie::getBoundingRect() const
{
    return getWorldTransform().transformRect(sf::FloatRect(20.f*2, 5.f*2, 42.f*2, 71.f*2));
}

void Zombie::setDirection(Direction dir)
{
    switch(dir)
    {
        case Left:
            setScale(1.f, 1.f);
            break;

        case Right:
        default:
            setScale(-1.f, 1.f);
            break;
    }

    Entity::setDirection(dir);
}

void Zombie::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(mCurrentBehavior)
    {
        AIState* nextState = mCurrentBehavior->run(*this);
        if(nextState)
        {
            setBehavior(nextState);
        }
    }

    // Update animations
    updateAnimation(dt);

    Entity::updateCurrent(dt, commands);
}

void Zombie::updateAnimation(sf::Time dt)
{
    if(mIsAttacking)
    {
        mAnimations[Attacking].update(dt);
    }
    else if(isDestroyed())
    {
        mAnimations[Dying].update(dt);
    }
    else
    {
        mAnimations[Walking].update(dt);
    }

    mMovingAnimation = isMoving();
}

void Zombie::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mIsAttacking)
    {
        target.draw(mAnimations[Attacking], states);
    }
    else if(isDestroyed())
    {
        target.draw(mAnimations[Dying], states);
    }
    else
    {
        target.draw(mAnimations[Walking], states);
    }

    sf::FloatRect bound(20.f*2, 5.f*2, 42.f*2, 71.f*2);
    sf::RectangleShape shape(sf::Vector2f(bound.width, bound.height));
    shape.setOutlineColor(sf::Color::Cyan);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color(255,255,255, 0));
    shape.setPosition(bound.left, bound.top);
    target.draw(shape, states);
}

