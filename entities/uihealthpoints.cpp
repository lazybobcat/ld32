#include "uihealthpoints.h"

Uihealthpoints::Uihealthpoints(TextureHolder &textures, Entity& ent) :
    SceneNode(Category::UI),
    mEntity(ent),
    mSprite(textures.get(Textures::Pvbar)),
    mBar(),
    mSize(sf::Vector2f((textures.get(Textures::Pvbar).getSize().x - 4)*3, (textures.get(Textures::Pvbar).getSize().y - 4)*3))
{
    mBar.setPosition(sf::Vector2f(6, 6));
    mBar.setSize(mSize);
    mBar.setFillColor(sf::Color::Green);
    mBar.setOutlineThickness(0.f);

    textures.get(Textures::Pvbar).setSmooth(false);
    mSprite.setScale(sf::Vector2f(3.f, 3.f));
}

void Uihealthpoints::updateCurrent(sf::Time /*dt*/, CommandQueue &/*commands*/)
{
    int pv = mEntity.getHealthpoints();
    if(pv < 0) pv = 0;
    sf::Vector2f size;
    size.y = mSize.y;
    size.x = mSize.x * pv / (float)mEntity.getMaxHealthpoints();

    mBar.setSize(size);
}

void Uihealthpoints::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mBar, states);
    target.draw(mSprite, states);
}
