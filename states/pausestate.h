#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <utils.h>
#include <states/state.h>
#include <SFML/Graphics.hpp>

class PauseState : public State
{
public:
    PauseState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    sf::Text        mText;
    sf::Text        mText2;
};

#endif // PAUSESTATE_H
