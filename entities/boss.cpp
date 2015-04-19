#include "boss.h"
#include <world.h>
#include <controllers/aicontroller.h>
#include <datatables.h>

#include <iostream>

Phase::Phase(World &world, Boss &boss, sf::Time duration) :
    mWorld(world),
    mBoss(boss),
    mDuration(duration),
    mElapsedTime(sf::Time::Zero)
{
}

void Phase::update(sf::Time dt, CommandQueue &commands)
{
    mElapsedTime += dt;

    for(auto& tuple : mSkills)
    {
        if(dt > std::get<0>(tuple))
        {
            std::get<0>(tuple) = std::get<1>(tuple);
            std::get<2>(tuple)(mBoss, dt);
        }
        else
        {
            std::get<0>(tuple) = std::get<0>(tuple) - dt;
        }
    }
}

bool Phase::isEnded() const
{
    return mElapsedTime >= mDuration;
}

void Phase::reset()
{
    mElapsedTime = sf::Time::Zero;
    for(auto& tuple : mSkills)
    {
        std::get<0>(tuple) = std::get<1>(tuple);
    }
}

void Phase::addSkill(Skill skill)
{
    mSkills.push_back(skill);
}

void Phase::addSkill(sf::Time cooldown, Action action)
{
    auto t = std::make_tuple(cooldown, cooldown, action);
    mSkills.push_back(t);
}



////////////////////////////////////////


Boss::Boss(World& world, AIController &ctrl, Bosses type, int hp) :
    Creature(ctrl, hp),
    mWorld(world),
    mCurrentPhase(0)
{
    std::vector<std::vector<Phase>>table = initializeBossPhases(world, *this);
    for(auto p : table[type])
    {
        addPhase(p);
    }
}

void Boss::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    //unknock();

    mPhases[mCurrentPhase].update(dt, commands);
    if(mPhases[mCurrentPhase].isEnded())
    {
        mPhases[mCurrentPhase].reset();
        nextPhase();
    }


    Creature::updateCurrent(dt, commands);
}

void Boss::addPhase(Phase phase)
{
    mPhases.push_back(std::move(phase));
}

void Boss::nextPhase()
{
    ++mCurrentPhase;
    if(mCurrentPhase >= mPhases.size())
    {
        mCurrentPhase = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////

FirstBoss::FirstBoss(World& world, AIController &ctrl, int hp, TextureHolder& textures) :
    Boss(world, ctrl, Bosses::Boss1, hp),
    mIsSensible(false)
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
    mAnimations[Attacking].setRepeating(true);
    mAnimations[Attacking].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Dying].setTexture(textures.get(Textures::ZombieDying));
    mAnimations[Dying].setFrameSize(sf::Vector2i(75,75));
    mAnimations[Dying].setNumFrames(4);
    mAnimations[Dying].setDuration(sf::seconds(0.4f));
    mAnimations[Dying].setRepeating(false);
    mAnimations[Dying].setScale(sf::Vector2f(2.f, 2.f));

    //

    mHorizontalVelocity = 170.f;
    mIsAttacking = true;
    setScale(0.75f, 0.75f);
}

void FirstBoss::attack()
{
    Creature::attack();
    mAnimations[Attacking].restart();
}

void FirstBoss::damage(int points)
{
    if(mIsSensible)
    {
        Creature::damage(points);
        std::cout << "Boss1 takes " << points << " damages" << std::endl;
    }
}

void FirstBoss::knock()
{
    // Unknockable if not sensible
    if(mIsSensible)
    {
        Creature::knock();
    }
}

void FirstBoss::unknock()
{
    Creature::unknock();
}

void FirstBoss::sensible()
{
    mAnimations[Attacking].stop();
    mWaitingSprite.setColor(sf::Color::Red);
    mIsAttacking = false;
    mAnimations[Attacking].restart();
    mIsSensible = true;
}

void FirstBoss::unsensible()
{
    Creature::unknock();
    mWaitingSprite.setColor(sf::Color(255,255,255, 255));
    mIsAttacking = true;
    mIsSensible = false;
}

bool FirstBoss::isDestroyed() const
{
    return mIsDying && mAnimations[Dying].isFinished() && mDepopTimer.asSeconds() >= 2.f;
}

sf::FloatRect FirstBoss::getBoundingRect() const
{
    //return getWorldTransform().transformRect(sf::FloatRect(20.f*2, 20.f*2, 42.f*2, 56.f*2));
    return getWorldTransform().transformRect(mWaitingSprite.getGlobalBounds());
}

void FirstBoss::setDirection(Direction dir)
{
    switch(dir)
    {
        case Left:
            setScale(0.75f, 0.75f);
            break;

        case Right:
        default:
            setScale(-0.75f, 0.75f);
            break;
    }

    Entity::setDirection(dir);
}

void FirstBoss::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(mIsDying)
    {
        mDepopTimer += dt;
    }

    // Update animations
    updateAnimation(dt);

    //mAIController.update(dt, commands, *this);
    Boss::updateCurrent(dt, commands);
}

void FirstBoss::updateAnimation(sf::Time dt)
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

void FirstBoss::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
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
}
