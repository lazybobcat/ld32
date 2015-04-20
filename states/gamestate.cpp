#include "gamestate.h"
#include <iostream>
#include <fstream>

GameState::GameState(StateStack &stack, Context context) :
    State(stack, context),
    mPlayer(*context.player),
    mWorld(*context.window, *context.textures, *context.fonts, *context.music, *context.sounds, *context.scripts, mPlayer),
    mIsGameOver(false)
{
    context.music->play(Musics::MainThemeMusical);
    context.music->setLoop(true);
    context.music->setVolume(10.f);
}


void GameState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    mWorld.draw();
}


bool GameState::update(sf::Time dt)
{

    // Mission status check
    if(mWorld.getPlayerEntity() && mWorld.getPlayerEntity()->isDestroyed())
    {
        mGameOverTimer += dt;

        if(!mIsGameOver)
        {
            mIsGameOver = true;
            std::fstream rfile("assets/score.txt", std::ios_base::in);
            if(rfile)
            {
                unsigned int bestScore;
                rfile >> bestScore;
                rfile.close();
                if(mWorld.getScore() > bestScore)
                {
                    rfile.open("assets/score.txt", std::ios_base::out | std::ios_base::trunc);
                    if(rfile)
                    {
                        rfile << mWorld.getScore();
                    }
                }
            }
        }

        if(mGameOverTimer.asSeconds() >= 1.2f)
        {
            requestStackClear();
            requestStackPush(States::Title);
            requestStackPush(States::GameOver);
        }
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    mWorld.update(dt);
    // If time, updte some scripted events

    return true;
}


bool GameState::handleEvent(const sf::Event &event)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                if(!mIsGameOver) requestStackPush(States::Pause);
                break;

            default:break;
        }
    }
    else if(event.type == sf::Event::LostFocus)
    {
        if(!mIsGameOver) requestStackPush(States::Pause);
    }

    return true;
}
