#include "gameoverstate.h"

GameoverState::GameoverState(StateStack &stack, Context context) :
    State(stack, context),
    mText()
{
    mText.setFont(context.fonts->get(Fonts::Savage));
    mText.setString("Game Over!");
    mText.setCharacterSize(50);
    mText.setColor(sf::Color::Red);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}

void GameoverState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(mText);
}


bool GameoverState::update(sf::Time dt)
{
    mTimer += dt;

    if(mTimer.asSeconds() >= 4.f)
    {
        requestStackPop();
    }

    return true;
}


bool GameoverState::handleEvent(const sf::Event &/*event*/)
{
    //CommandQueue& commands = mWorld.getCommandQueue();

    return true;
}
