#include <application.h>
#include <utils.h>
#include <states/titlestate.h>
#include <states/gamestate.h>
#include <states/pausestate.h>
#include <states/gameoverstate.h>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application(unsigned int width, unsigned int height, const std::string &title) :
    mWindow(sf::VideoMode(width,height), title.c_str(), sf::Style::Close),
    mTextures(),
    mFonts(),
    mSounds(),
    mMusic(),
    mScripts(),
    mPlayer(mWindow),
    mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mMusic, mScripts, mPlayer)),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0)
{
    mWindow.setPosition(sf::Vector2i(0,0));
    mWindow.setKeyRepeatEnabled(false);
    mWindow.setMouseCursorVisible(false);

    // Resources
    mFonts.load(Fonts::Main, "assets/fonts/HandTIMES.ttf");
    mFonts.load(Fonts::Savage, "assets/fonts/savage.ttf");
    mTextures.load(Textures::Particle,      "assets/textures/particle.png");
    mTextures.load(Textures::Background,    "assets/textures/background.png");
    mTextures.load(Textures::BackgroundMenu,"assets/textures/background-menu.png");
    mTextures.load(Textures::Hero,          "assets/textures/hero.png");
    mTextures.load(Textures::HeroWalkingWith,"assets/textures/hero-walkingwith.png");
    mTextures.load(Textures::HeroWalkingWithout,"assets/textures/hero-walkingwithout.png");
    mTextures.load(Textures::HeroJumpingWith,"assets/textures/hero-jumpingwith.png");
    mTextures.load(Textures::HeroJumpingWithout,"assets/textures/hero-jumpingwithout.png");
    mTextures.load(Textures::Platforms,     "assets/textures/platforms.png");
    mTextures.load(Textures::Unicorn,       "assets/textures/unicorn.png");
    mTextures.load(Textures::ZombieAttacking,"assets/textures/zombies-attacking.png");
    mTextures.load(Textures::ZombieWalking, "assets/textures/zombies-walking.png");
    mTextures.load(Textures::ZombieDying,   "assets/textures/zombies-dying.png");
    mTextures.load(Textures::Cursor,        "assets/textures/unicorn_cursor.png");
    mTextures.load(Textures::Pvbar,         "assets/textures/pvbar.png");
    mTextures.load(Textures::Medkit,        "assets/textures/medkit.png");
    mScripts.registerFile(Scripts::HelloWorld, "assets/scripts/helloworld.lua");


    mCursor.setTexture(mTextures.get(Textures::Cursor));

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(5.f,5.f);
    mStatisticsText.setCharacterSize(10);

    registerStates();
    mStateStack.pushState(States::Title);
}



void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        sf::Vector2i mp = sf::Mouse::getPosition(mWindow);
        mCursor.setPosition(sf::Vector2f(mp.x * 1.f, mp.y * 1.f));

        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(elapsedTime);
        render();
    }
}

void Application::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);
        if(event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();
    mStateStack.draw();

    mWindow.draw(mCursor);
    mWindow.setView(mWindow.getDefaultView());
    //mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<GameoverState>(States::GameOver);
    /*mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<LoadingState>(States::Loading);
    */
}

void Application::updateStatistics(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if(mStatisticsUpdateTime >= sf::seconds(1.f))
    {
        mStatisticsText.setString(
                    "Frames/sec = " + toString(mStatisticsNumFrames) + "\n" +
                    "Time/update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us"
        );

        mStatisticsNumFrames = 0;
        mStatisticsUpdateTime -= sf::seconds(1.f);
    }
}
