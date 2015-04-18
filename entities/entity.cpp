#include <entities/entity.h>
#include <entities/scenenode.h>
#include <events/commandqueue.h>

Entity::Entity(int healthpoints) :
    SceneNode(Category::None),
    mIsJumping(false),
    mVerticalVelocity(0.f),
    mHorizontalVelocity(300.f),
    mHealthpoints(healthpoints),
    mJustGetHurt(false),
    mLastHurtTime(sf::Time::Zero),
    mKnocked(false),
    mDirection(Left),
    mIsMoving(false)
{
}

void Entity::updateCurrent(sf::Time dt, CommandQueue &/*commands*/)
{
    updatePhysics(dt);

    if(isDestroyed())
            return;

    if(mJustGetHurt)
    {
        mLastHurtTime += dt;
        if(mLastHurtTime > sf::seconds(0.8f))
        {
            mJustGetHurt = false;
            mLastHurtTime = sf::Time::Zero;
            mKnocked = false;
        }
    }

    if(mIsMoving)
    {
        mLastSafePosition = getWorldPosition();
        switch(mDirection)
        {
            case Left:
                SceneNode::move(-sf::Vector2f(mHorizontalVelocity * dt.asSeconds(), 0.f));
                break;

            case Right:
                SceneNode::move(sf::Vector2f(mHorizontalVelocity * dt.asSeconds(), 0.f));
                break;
        }
        mIsMoving = false;
    }
}

void Entity::applyPhysics(sf::Time dt, float gravity, sf::RenderWindow& window)
{
    float delta = gravity*dt.asSeconds();
    mVerticalVelocity += delta;
    if(mVerticalVelocity >= (delta * 1.25f)) // <==> If falling for more than 0.25sec
    {
        mIsJumping = true;
    }

    sf::Vector2f pos = getPosition();

    if(pos.x < 0)
    {
        pos.x = 0;
    }
    else if(pos.x > window.getSize().x)
    {
        pos.x = window.getSize().x;
    }

    setPosition(pos);
}

void Entity::updatePhysics(sf::Time dt)
{
    auto pos = getPosition();
    pos.y = pos.y + mVerticalVelocity * dt.asSeconds();
    setPosition(pos);
}



void Entity::move(Direction dir)
{
    if(isKnocked()) return;

    setDirection(dir);
    mIsMoving = true;
}

bool Entity::isMoving() const
{
    return mIsMoving;
}

void Entity::demove()
{
    setPosition(mLastSafePosition);
}

void Entity::stop()
{
    mIsMoving = false;
}


void Entity::setDirection(Direction dir)
{
    mDirection = dir;
}

Entity::Direction Entity::getDirection() const
{
    return mDirection;
}



void Entity::knock()
{
    mKnocked = true;
}

void Entity::unknock()
{
    mKnocked = false;
}

bool Entity::isKnocked() const
{
    return mKnocked;
}



void Entity::damage(int points)
{
    mHealthpoints -= points;
}

void Entity::heal(int points)
{
    mHealthpoints += points;
}

void Entity::destroy()
{
    mHealthpoints = 0;
}

int Entity::getHealthpoints() const
{
    return mHealthpoints;
}

bool Entity::isDestroyed() const
{
    return (mHealthpoints <= 0);
}

void Entity::playLocalSound(CommandQueue &commands, Sounds::ID sound)
{
    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, sound, getWorldPosition()));

    commands.push(command);
}
