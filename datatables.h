#ifndef DATATABLES_H
#define DATATABLES_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>

#include <resources/resourceidentifiers.h>
#include <entities/boss.h>

// Example : this structure contains each information about a particle type
struct ParticleData
{
    sf::Color   color;
    sf::Time    lifetime;
};

// This function inits each particle types with its basic data
std::vector<ParticleData>	initializeParticleData();

std::vector<std::vector<Phase> > initializeBossPhases(World &world, Boss &boss);


#endif // DATATABLES_H
