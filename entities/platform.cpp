#include "platform.h"

Platform::Platform(Platform::Type type, TextureHolder& textures) : Entity(1)
{
    mSprite.setTexture(textures.get(Textures::Platforms));

    switch(type)
    {
        case Small:
            mSprite.setTextureRect(sf::IntRect(0,0,174,45));
            break;

        case Medium:
            mSprite.setTextureRect(sf::IntRect(0,45,376,45));
            break;

        case Ground:
            mSprite.setTextureRect(sf::IntRect(0,90,1280,45));
    }
}


sf::FloatRect Platform::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Platform::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
