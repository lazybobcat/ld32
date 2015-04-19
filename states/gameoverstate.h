#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <utils.h>
#include <states/state.h>
#include <SFML/Graphics.hpp>

class GameoverState : public State
{
public:
    GameoverState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    sf::Text        mText;
    sf::Time        mTimer;
};

#endif // GAMEOVERSTATE_H
