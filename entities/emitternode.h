#ifndef EMITTERNODE_H
#define EMITTERNODE_H

#include <entities/scenenode.h>
#include <resources/particle.h>
#include <events/commandqueue.h>

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
    enum EmitterType
    {
        Rainbow,
        Blood
    };

public:
    explicit EmitterNode(Particle::Type type, EmitterType etype);

private:
    virtual void    updateCurrent(sf::Time dt, CommandQueue &commands);

    void            emitParticles(sf::Time dt);

private:
    sf::Time        mAccumulatedTime;
    Particle::Type  mType;
    EmitterType     mEType;
    ParticleNode*   mParticleSystem;


};

#endif // EMITTERNODE_H
