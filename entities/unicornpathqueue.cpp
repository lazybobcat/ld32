#include "unicornpathqueue.h"
#include <utils.h>

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
    if(mPlayer && !mPlayer->canFire()) return mPoints;

    if(mPoints.size() == 0 && mPlayer)
    {
        auto ppos = mPlayer->getPosition();
        ppos.y -= 25.f;

        mPoints.push_back(ppos);
    }

    if(mPoints.size() < mPointsLimit)
    {
        if(mPoints.size() > 0)
        {
            float dist = distance(mPoints.back(), point);
            if(dist >= mPointsSpace)
            {
                point = mPoints.back() + (mPointsSpace * unitVector(point - mPoints.back()));
                mPoints.push_back(point);
            }
        }
        else
        {
            mPoints.push_back(point);
        }

        if(mPlayer)
        {
            mPlayer->setCanMove(false);
        }
    }

    return mPoints;
}

void UnicornPathQueue::clear()
{
    mPoints.clear();

    if(mPlayer)
    {
        mPlayer->setCanMove(true);
    }
}


void UnicornPathQueue::drawCurrent(sf::RenderTarget &target, sf::RenderStates /*states*/) const
{
    if(mPoints.size() < 2) return;

    sf::VertexArray lines(sf::LinesStrip, mPoints.size());
    for(std::size_t i = 0; i < mPoints.size(); ++i)
    {
        lines[i].position = mPoints[i];
        lines[i].color = sf::Color(0,0,0, 255);
    }
    target.draw(lines);
}
