#include "zombie.h"
#include <controllers/aicontroller.h>
#include <controllers/AIStates/aistate.h>

Zombie::Zombie(TextureHolder &textures, AIController &controller) :
    Entity(20),
    mIsResting(false),
    mIsAttacking(false),
    mMovingAnimation(false),
    mIsDying(false),
    mAIController(controller),
    mCurrentBehavior(nullptr)
{
    textures.get(Textures::ZombieWalking).setSmooth(false);
    textures.get(Textures::ZombieAttacking).setSmooth(false);
    textures.get(Textures::ZombieDying).setSmooth(false);

    mWaitingSprite.setTexture(textures.get(Textures::ZombieWalking));
    mWaitingSprite.setTextureRect(sf::IntRect(0,0,75,75));
    mWaitingSprite.setScale(2.f, 2.f);

    mDeadSprite.setTexture(textures.get(Textures::ZombieDying));
    mDeadSprite.setTextureRect(sf::IntRect(75*3,0,75,75));
    mDeadSprite.setScale(2.f, 2.f);

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
    mAnimations[Dying].setDuration(sf::seconds(0.4f));
    mAnimations[Dying].setRepeating(false);
    mAnimations[Dying].setScale(sf::Vector2f(2.f, 2.f));

    //

    mHorizontalVelocity = 150.f;
}


Zombie::~Zombie()
{
    if(mCurrentBehavior)
    {
        mCurrentBehavior->deinit(*this);
        delete(mCurrentBehavior);
    }
}


void Zombie::jump()
{
    if(!mIsJumping)
    {
        sf::Vector2f pos = getPosition();
        pos.y -= 2.f;
        setPosition(pos); // Hack to avoid unwated collisions with the platform
        mVerticalVelocity = -450;
        mIsJumping = true;
        rest();
    }
}

void Zombie::attack()
{
    mIsAttacking = true;
    mAnimations[Attacking].restart();
}

void Zombie::damage(int points)
{
    Entity::damage(points);

    if(mHealthpoints <= 0) die();
}

void Zombie::knock()
{
    Entity::knock();
    stop();
    //mIsAttacking = false;
    mAnimations[Attacking].stop();
    mWaitingSprite.setColor(sf::Color::Red);
}

void Zombie::unknock()
{
    Entity::unknock();
    mWaitingSprite.setColor(sf::Color(255,255,255, 255));
}

bool Zombie::isResting() const
{
    return mIsResting;
}

void Zombie::rest()
{
    if(!mIsResting)
    {
        mIsResting = true;
        mRestingTime = sf::Time::Zero;
    }
}

void Zombie::unrest()
{
    mIsResting = false;
    mRestingTime = sf::Time::Zero;
}

void Zombie::die()
{
    mIsDying = true;
}


bool Zombie::isDestroyed() const
{
    return mIsDying && mAnimations[Dying].isFinished() && mDepopTimer.asSeconds() >= 2.f;
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
    return getWorldTransform().transformRect(sf::FloatRect(20.f*2, 20.f*2, 42.f*2, 56.f*2));
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
    if(mIsDying)
    {
        mDepopTimer += dt;
    }
    else if(mCurrentBehavior)
    {
        AIState* nextState = mCurrentBehavior->run(*this, dt, commands);
        if(nextState)
        {
            setBehavior(nextState);
        }
    }

    if(mIsResting)
    {
        mRestingTime += dt;
        if(mRestingTime.asSeconds() >= 0.8f)
            unrest();
    }

    // Update animations
    updateAnimation(dt);

    mAIController.update(dt, commands, *this);
    Entity::updateCurrent(dt, commands);
}

void Zombie::updateAnimation(sf::Time dt)
{
    if(mIsAttacking)
    {
        mAnimations[Attacking].update(dt);
    }
    else if(mIsDying)
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
    if(isDestroyed()) return;

    if(mIsAttacking)
    {
        target.draw(mAnimations[Attacking], states);
    }
    else if(mIsDying)
    {
        if(!mAnimations[Dying].isFinished())
            target.draw(mAnimations[Dying], states);
        else
            target.draw(mDeadSprite, states);
    }
    else if(mMovingAnimation)
    {
        target.draw(mAnimations[Walking], states);
    }
    else
    {
        target.draw(mWaitingSprite, states);
    }

    /*sf::FloatRect bound(20.f*2, 20.f*2, 42.f*2, 56.f*2);
    sf::RectangleShape shape(sf::Vector2f(bound.width, bound.height));
    shape.setOutlineColor(sf::Color::Cyan);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color(255,255,255, 0));
    shape.setPosition(bound.left, bound.top);
    target.draw(shape, states);*/
}

