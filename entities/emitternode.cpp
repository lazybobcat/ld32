#include <entities/emitternode.h>
#include <entities/particlenode.h>
#include <utils.h>

EmitterNode::EmitterNode(Particle::Type type, EmitterType etype) :
    SceneNode(),
    mAccumulatedTime(sf::Time::Zero),
    mType(type),
    mEType(etype),
    mParticleSystem(nullptr)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(mParticleSystem)
    {
        emitParticles(dt);
    }
    else
    {
        auto finder = [this](ParticleNode& container, sf::Time) {
            if(container.getParticleType() == mType)
                mParticleSystem = &container;
        };
        Command command;
        command.action = derivedAction<ParticleNode>(finder);
        command.category = Category::ParticleSystem;
        commands.push(command);
    }
}

void EmitterNode::emitParticles(sf::Time dt)
{
    const float emissionRate = ((mEType == Rainbow) ? 30.f : 20.f);
    const sf::Time interval = sf::seconds(1.f) / emissionRate;

    mAccumulatedTime += dt;

    while(mAccumulatedTime > interval)
    {
        mAccumulatedTime -= interval;
        if(mEType == Rainbow)
        {
            mParticleSystem->addParticle(sf::Color::Red, getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-20.f, 20.f)));
            mParticleSystem->addParticle(sf::Color(255, 127, 0), getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-20.f, 20.f)));
            mParticleSystem->addParticle(sf::Color::Yellow, getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-20.f, 20.f)));
            mParticleSystem->addParticle(sf::Color::Green, getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-20.f, 20.f)));
            mParticleSystem->addParticle(sf::Color::Blue, getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-20.f, 20.f)));
            mParticleSystem->addParticle(sf::Color(75, 0, 130), getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-20.f, 20.f)));
        }
        else
        {
            mParticleSystem->addParticle(sf::Color(184, 17, 17), getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-10.f, 10.f)));
            //mParticleSystem->addParticle(sf::Color(184, 17, 17), getWorldPosition() + sf::Vector2f(randomFloat(-20.f, 20.f), randomFloat(-10.f, 10.f)));
        }
    }
}
