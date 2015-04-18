#include <world.h>
#include <algorithm>

#include <iostream>

////////////////////////////////////////////////////

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    if(type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if(type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }

    return false;
}

////////////////////////////////////////////////////


World::World(sf::RenderWindow& window, TextureHolder &textures, FontHolder &fonts, MusicPlayer &music, SoundPlayer &sounds, ScriptPlayer &scripts, PlayerController &player) :
    mWindow(window),
    mWorldView(window.getDefaultView()),
    mTextures(textures),
    mFonts(fonts),
    mMusic(music),
    mSounds(sounds),
    mScripts(scripts),
    mPlayer(player),
    mSceneGraph(),
    mSceneLayers()
{
    loadTextures();
    buildScene();

    // Other things here, like setting the view center on the player, scores, etc...
}


void World::loadTextures()
{
    //mTextures.load(Textures::MyTextureIDHere, "assets/textures/whatever.png");
}

void World::buildScene()
{
    // Initialize layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Background
    std::unique_ptr<SpriteNode> background(new SpriteNode(mTextures.get(Textures::Background)));
    mSceneLayers[Background]->attachChild(std::move(background));

    // Platforms
    std::unique_ptr<Platform> platform_ground(new Platform(Platform::Ground, mTextures));
    platform_ground->setPosition(sf::Vector2f(0, mWindow.getSize().y - 45));
    mSceneLayers[Foreground]->attachChild(std::move(platform_ground));

    std::unique_ptr<Platform> platform_medium1(new Platform(Platform::Medium, mTextures));
    platform_medium1->setPosition(sf::Vector2f(0, 430));
    mSceneLayers[Foreground]->attachChild(std::move(platform_medium1));

    std::unique_ptr<Platform> platform_medium2(new Platform(Platform::Medium, mTextures));
    platform_medium2->setPosition(sf::Vector2f(450, 225));
    mSceneLayers[Foreground]->attachChild(std::move(platform_medium2));

    std::unique_ptr<Platform> platform_small1(new Platform(Platform::Small, mTextures));
    platform_small1->setPosition(sf::Vector2f(1106, 430));
    mSceneLayers[Foreground]->attachChild(std::move(platform_small1));

    // Add particle node to the scene

    // Player
    std::unique_ptr<Player> player(new Player(mTextures));
    player->setPosition(640, 475);
    player->setOrigin(75, 150);
    mPlayerEntity = player.get();
    mSceneLayers[Foreground]->attachChild(std::move(player));

    // Ennemies

    // ...
}

void World::update(sf::Time dt)
{
    // Game logic here

    // Forward commands to scene
    while(!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    // Dirty : applying gravity and bounds here
    if(mPlayerEntity)
    {
        mPlayerEntity->mVerticalVelocity += 1000*dt.asSeconds();
        sf::Vector2f pos = mPlayerEntity->getPosition();

        if(pos.x < 0)
        {
            pos.x = 0;
        }
        else if(pos.x > mWindow.getSize().x)
        {
            pos.x = mWindow.getSize().x;
        }

        mPlayerEntity->setPosition(pos);
    }

    // Handle collisions
    handleCollisions();

    // Set the listener position

    mSceneGraph.update(dt);
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for(SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::Player, Category::Enemy))
        {
            /*auto& player = static_cast<PlayerEntity&>(*pair.first);
            auto& enemy = static_cast<Creature&>(*pair.second);*/
        }
        else if (matchesCategories(pair, Category::Player, Category::Platform))
        {
            auto& player = static_cast<Player&>(*pair.first);
            auto& platform = static_cast<Platform&>(*pair.second);

            player.mIsJumping = false;
            player.mVerticalVelocity = 0.f;
            sf::Vector2f pos = player.getPosition();
            pos.y = platform.getPosition().y;
            player.setPosition(pos);
        }
    }
}


void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}
