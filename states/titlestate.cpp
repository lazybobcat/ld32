#include <states/titlestate.h>
#include <utils.h>
#include <iostream>
#include <fstream>


TitleState::TitleState(StateStack &stack, Context context) :
    State(stack, context),
    mText(),
    mPlayer(*context.player),
    mWorld(*context.window, *context.textures, *context.fonts, *context.music, *context.sounds, *context.scripts, mPlayer, true)
{
    mText.setFont(context.fonts->get(Fonts::Savage));
    mText.setString("Unikorn");
    mText.setCharacterSize(70);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize().x / 2.f, 50.f);

    unsigned int bestScore = 0;
    std::fstream rfile("assets/score.txt", std::ios_base::in);
    if(rfile)
    {
        rfile >> bestScore;
        rfile.close();
    }

    mScoreText.setFont(context.fonts->get(Fonts::Savage));
    mText.setString("Best Score: "+toString(bestScore));
    mText.setCharacterSize(30);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y - 23.f);

    context.music->play(Musics::MainThemeSinging);
    context.music->setLoop(false);
    context.music->setVolume(20.f);

    context.window->setKeyRepeatEnabled(false);
}


void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    mWorld.draw();
    window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    mWorld.update(dt);

    sf::FloatRect window(905,105,250,138);
    if(window.intersects(mWorld.getPlayerEntity()->getBoundingRect()))
    {
        requestStackClear();
        requestStackPush(States::Game);
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Return:
                requestStackPop();
                requestStackPush(States::Game);
                break;

            case sf::Keyboard::Escape:
                requestStackClear();
                break;

            default:break;
        }
    }

    return true;
}
