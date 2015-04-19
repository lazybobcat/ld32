#ifndef BOSS_H
#define BOSS_H

#include <entities/creature.h>
#include <tuple>
#include <vector>
#include <functional>

class Boss;
class World;

class Phase
{
public:
    typedef std::function<void(Boss&,sf::Time)> Action;
    typedef std::tuple<sf::Time,sf::Time,Action> Skill;

public:
    Phase(World& world, Boss& boss, sf::Time duration);

    void update(sf::Time dt, CommandQueue& commands);
    bool isEnded() const;
    void reset();

    void addSkill(Skill skill);
    void addSkill(sf::Time cooldown, Action action);

private:
    World&          mWorld;
    Boss&           mBoss;
    sf::Time        mDuration;
    sf::Time        mElapsedTime;

    std::vector<Skill> mSkills;
};

class Boss : public Creature
{
public:
    enum Bosses
    {
        Boss1,
        Boss2,

        TypeCount
    };

public:
    Boss(World& world, AIController &ctrl, Bosses type, int hp);

    void    addPhase(Phase phase);

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);

    void         nextPhase();

private:
    World&          mWorld;

    std::vector<Phase>  mPhases;
    std::size_t         mCurrentPhase;
};

#endif // BOSS_H
