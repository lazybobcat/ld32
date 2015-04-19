#ifndef UIHEALTHPOINTS_H
#define UIHEALTHPOINTS_H

#include <entities/scenenode.h>
#include <entities/entity.h>

class Uihealthpoints : public SceneNode
{
public:
    Uihealthpoints(TextureHolder &textures, Entity& ent);


private:
    virtual void    updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void    drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Entity&         mEntity;
    sf::Sprite      mSprite;
    sf::RectangleShape mBar;
    sf::Vector2f    mSize;
};

#endif // UIHEALTHPOINTS_H
