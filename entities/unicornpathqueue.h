#ifndef UNICORNPATHQUEUE_H
#define UNICORNPATHQUEUE_H

#include <utils.h>
#include <entities/scenenode.h>
#include <entities/player.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

class UnicornPathQueue : public SceneNode
{
public:
    UnicornPathQueue(Player* player);

    std::vector<sf::Vector2f>&  addPoint(sf::Vector2f point);
    void                        clear();

private:
    virtual void    drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Player*                     mPlayer;
    std::vector<sf::Vector2f>   mPoints;
    std::size_t                 mPointsLimit;
    float                       mPointsSpace;
};

#endif // UNICORNPATHQUEUE_H
