#include "boss.h"
#include <world.h>
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
    unknock();

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
