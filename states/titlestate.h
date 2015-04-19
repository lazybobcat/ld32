#ifndef TITLESTATE_H
#define TITLESTATE_H

#include <SFML/Graphics.hpp>

#include <states/state.h>
#include <world.h>
#include <controllers/playercontroller.h>

class TitleState : public State
{
public:
    TitleState(StateStack& stack, Context context);
    virtual void        draw();
    virtual bool        update(sf::Time dt);
    virtual bool        handleEvent(const sf::Event &event);

private:
    sf::Text            mText;

    PlayerController&   mPlayer;
    World               mWorld;
};

#endif // TITLESTATE_H
