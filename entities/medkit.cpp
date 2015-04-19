#include "medkit.h"

#include <iostream>

Medkit::Medkit(TextureHolder &textures) :
    mUsed(false)
{
    mSprite.setTexture(textures.get(Textures::Medkit));

    std::cout << "medkit created" << std::endl;
}


void Medkit::use(Entity &on)
{
    on.heal(25);
    mUsed = true;
}
