#include "unicornpathqueue.h"

#include <iostream>


////////////////////////////////////////////////////////////

float distance(const sf::Vector2f &lhs, const sf::Vector2f &rhs)
{
    return length(lhs - rhs);
}

////////////////////////////////////////////////////////////


UnicornPathQueue::UnicornPathQueue(Player *player) :
    SceneNode(Category::UnicornPath),
    mPlayer(player),
    mPointsLimit(10),
    mPointsSpace(100.f)
{
    clear();
}


std::vector<sf::Vector2f>& UnicornPathQueue::addPoint(sf::Vector2f point)
{
    if(mPoints.size() == 0 && mPlayer)
    {
        auto ppos = mPlayer->getPosition();
        ppos.y -= 25.f;

        if(distance(ppos, point) > mPointsSpace)
        {
            return mPoints;
        }


        mPoints.push_back(ppos);
    }

    if(mPoints.size() < mPointsLimit)
    {
        if(mPoints.size() > 0)
        {
            float dist = distance(mPoints.back(), point);
            if(dist >= mPointsSpace && dist < mPointsSpace*1.25f)
            {
                mPoints.push_back(point);
            }
        }
        else
        {
            mPoints.push_back(point);
        }
    }

    return mPoints;
}

void UnicornPathQueue::clear()
{
    mPoints.clear();
}


void UnicornPathQueue::drawCurrent(sf::RenderTarget &target, sf::RenderStates /*states*/) const
{
    if(mPoints.size() < 2) return;

    sf::VertexArray lines(sf::LinesStrip, mPoints.size());
    for(std::size_t i = 0; i < mPoints.size(); ++i)
    {
        lines[i].position = mPoints[i];
        lines[i].color = sf::Color(255,0,0, 125);
    }
    target.draw(lines);
}
