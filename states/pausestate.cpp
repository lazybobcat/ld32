#include "pausestate.h"

PauseState::PauseState(StateStack &stack, Context context) :
    State(stack, context),
    mText(),
    mText2()
{
    mText.setFont(context.fonts->get(Fonts::Savage));
    mText.setString("The game is paused!");
    mText.setCharacterSize(50);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize().x / 2.f, 200.f);

    mText2.setFont(context.fonts->get(Fonts::Savage));
    mText2.setString("Press ESC to come back to the game\nPress M to go to the main menu\nPress Q to Quit the game");
    mText2.setCharacterSize(40);
    centerOrigin(mText2);
    mText2.setPosition(context.window->getView().getSize().x / 2.f, 400.f);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    sf::RectangleShape back;
    back.setFillColor(sf::Color(0,0,0, 200));
    back.setSize(window.getView().getSize());

    window.draw(back);
    window.draw(mText);
    window.draw(mText2);
}


bool PauseState::update(sf::Time /*dt*/)
{
    return false;
}


bool PauseState::handleEvent(const sf::Event &event)
{
    //CommandQueue& commands = mWorld.getCommandQueue();

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                requestStackPop();
                break;

            case sf::Keyboard::M:
                requestStackClear();
                requestStackPush(States::Title);
                break;

            case sf::Keyboard::Q:
                requestStackClear();
                break;

            default:break;
        }
    }

    return false;
}
