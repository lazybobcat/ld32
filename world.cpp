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


World::World(sf::RenderWindow& window, TextureHolder &textures, FontHolder &fonts, MusicPlayer &music, SoundPlayer &sounds, ScriptPlayer &scripts, PlayerController &player, bool menuworld) :
    mWindow(window),
    mWorldView(window.getDefaultView()),
    mTextures(textures),
    mFonts(fonts),
    mMusic(music),
    mSounds(sounds),
    mScripts(scripts),
    mPlayer(player),
    mSceneGraph(),
    mSceneLayers(),
    mPlayerEntity(nullptr),
    mAIController(),
    mPoints(0),
    mPointsText(nullptr)
{
    loadTextures();
    if(menuworld)   buildMenuScene();
    else            buildScene();

    // Other things here, like setting the view center on the player, scores, etc...
    std::unique_ptr<SoundNode> sound(new SoundNode(sounds));
    mSceneGraph.attachChild(std::move(sound));

    std::unique_ptr<CameraNode> camera(new CameraNode(mWorldView));
    mSceneGraph.attachChild(std::move(camera));
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
        Category::Type cat = ((i == Foreground) ? Category::Foreground : Category::None);
        SceneNode::Ptr layer(new SceneNode(cat));
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
    // Adding Unicorn Path Queue
    std::unique_ptr<UnicornPathQueue> path(new UnicornPathQueue(mPlayerEntity));
    mSceneLayers[Foreground]->attachChild(std::move(path));
    mSceneLayers[Foreground]->attachChild(std::move(player));


    // Ennemies
    std::unique_ptr<Zombie> zombie1(new Zombie(mTextures, mAIController));
    zombie1->setPosition(610, 100);
    zombie1->setOrigin(75, 150);
    mSceneLayers[Foreground]->attachChild(std::move(zombie1));


    // UI
    std::unique_ptr<Uihealthpoints> php(new Uihealthpoints(mTextures, *mPlayerEntity));
    php->setPosition(40, (int)mWindow.getSize().y - 35);
    mSceneLayers[UI]->attachChild(std::move(php));

    std::unique_ptr<TextNode> score(new TextNode("Score: " + toString(mPoints), mFonts, 30));
    score->setPosition(1200-90, (int)mWindow.getSize().y - 23);
    mPointsText = score.get();
    mSceneLayers[UI]->attachChild(std::move(score));
}

void World::buildMenuScene()
{
    // Initialize layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        Category::Type cat = ((i == Foreground) ? Category::Foreground : Category::None);
        SceneNode::Ptr layer(new SceneNode(cat));
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Background
    std::unique_ptr<SpriteNode> background(new SpriteNode(mTextures.get(Textures::BackgroundMenu)));
    mSceneLayers[Background]->attachChild(std::move(background));

    // Platforms
    std::unique_ptr<Platform> platform_ground(new Platform(Platform::Ground, mTextures));
    platform_ground->setPosition(sf::Vector2f(0, mWindow.getSize().y - 45));
    mSceneLayers[Foreground]->attachChild(std::move(platform_ground));

    std::unique_ptr<Platform> platform_small1(new Platform(Platform::Small, mTextures));
    platform_small1->setPosition(sf::Vector2f(1106, 430));
    mSceneLayers[Foreground]->attachChild(std::move(platform_small1));

    // Add particle node to the scene

    // Player
    std::unique_ptr<Player> player(new Player(mTextures));
    player->setPosition(640, 10);
    player->setOrigin(75, 150);
    mPlayerEntity = player.get();
    // Adding Unicorn Path Queue
    std::unique_ptr<UnicornPathQueue> path(new UnicornPathQueue(mPlayerEntity));
    mSceneLayers[Foreground]->attachChild(std::move(path));
    mSceneLayers[Foreground]->attachChild(std::move(player));
}


void World::update(sf::Time dt)
{
    float gravity = 1000.f;

    // Game logic here
    Command gravity_player;
    gravity_player.action = derivedAction<Player>([&](Player& player, sf::Time dt) {
        player.applyPhysics(dt, gravity, mWindow);
    });
    gravity_player.category = Category::Player;
    mCommandQueue.push(gravity_player);

    Command gravity_unicorn;
    gravity_unicorn.action = derivedAction<Unicorn>([&](Unicorn& unicorn, sf::Time dt) {
        if(!unicorn.isTraveling()) unicorn.applyPhysics(dt, gravity, mWindow);
    });
    gravity_unicorn.category = Category::Unicorn;
    mCommandQueue.push(gravity_unicorn);

    Command gravity_enemies;
    gravity_enemies.action = derivedAction<Zombie>([&](Zombie& zombie, sf::Time dt) {
        zombie.applyPhysics(dt, gravity, mWindow);
    });
    gravity_enemies.category = Category::Enemy;
    mCommandQueue.push(gravity_enemies);



    // Forward commands to scene
    while(!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    // Handle collisions
    handleCollisions();

    //if(mPlayerEntity->isDestroyed()) return;


    // Score
    if(mPointsText)
    {
        mPointsText->setText("Score: " + toString(mPoints));
    }

    // Remove useless entities
    mSceneGraph.removeWrecks();

    // Set the listener position

    mSceneGraph.update(dt, mCommandQueue);
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for(SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::Player, Category::Platform))
        {
            auto& player = static_cast<Player&>(*pair.first);
            auto& platform = static_cast<Platform&>(*pair.second);

            player.mIsJumping = false;
            player.mVerticalVelocity = 0.f;
            sf::Vector2f pos = player.getPosition();
            pos.y = platform.getPosition().y;
            player.setPosition(pos);
        }
        else if (matchesCategories(pair, Category::Enemy, Category::Platform))
        {
            auto& zombie = static_cast<Zombie&>(*pair.first);
            auto& platform = static_cast<Platform&>(*pair.second);

            zombie.mIsJumping = false;
            zombie.mVerticalVelocity = 0.f;
            sf::Vector2f pos = zombie.getPosition();
            pos.y = platform.getPosition().y;
            zombie.setPosition(pos);
        }
        else if (matchesCategories(pair, Category::Unicorn, Category::Enemy))
        {
            auto& unicorn = static_cast<Unicorn&>(*pair.first);
            auto& zombie = static_cast<Zombie&>(*pair.second);

            if(unicorn.isTraveling())
            {
                if(!zombie.isKnocked())
                {
                    zombie.damage(unicorn.getAttackPower());
                    zombie.knock();
                    shakeCamera();

                    if(zombie.getHealthpoints() <= 0)
                    {
                        mPoints += zombie.getMaxHealthpoints();
                    }
                }
            }
        }
        else if (matchesCategories(pair, Category::Unicorn, Category::Platform))
        {
            auto& unicorn = static_cast<Unicorn&>(*pair.first);
            auto& platform = static_cast<Platform&>(*pair.second);

            if(!unicorn.isTraveling())
            {
                unicorn.mIsJumping = false;
                unicorn.mVerticalVelocity = 0.f;
                sf::Vector2f pos = unicorn.getPosition();
                pos.y = platform.getPosition().y;
                unicorn.setPosition(pos);
            }
        }
        else if (matchesCategories(pair, Category::Player, Category::Unicorn))
        {
            auto& player = static_cast<Player&>(*pair.first);
            auto& unicorn = static_cast<Unicorn&>(*pair.second);

            if(!unicorn.isDestroyed() && !unicorn.isTraveling())
            {
                player.retrieve();
                unicorn.retrieve();
            }
        }
        else if (matchesCategories(pair, Category::Player, Category::Enemy))
        {
            auto& player = static_cast<Player&>(*pair.first);
            auto& zombie = static_cast<Zombie&>(*pair.second);

            if(!player.isDestroyed() && !zombie.isDestroyed() && zombie.isAttacking())
            {
                if(!player.isKnocked())
                {
                    player.damage(zombie.getAttackPower());
                    player.knock();
                    splashBlood();
                }

            }
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

void World::shakeCamera()
{
    Command command;
    command.category = Category::Camera;
    command.action = derivedAction<CameraNode>([&](CameraNode& c, sf::Time) {
        c.shake(sf::seconds(0.6f));
    });
    mCommandQueue.push(command);

    sf::sleep(sf::milliseconds(40));
}

void World::shakeCameraFor(float time)
{
    Command command;
    command.category = Category::Camera;
    command.action = derivedAction<CameraNode>([=](CameraNode& c, sf::Time) {
        c.shake(sf::seconds(time));
    });
    mCommandQueue.push(command);
}

void World::splashBlood()
{
    Command command;
    command.category = Category::Camera;
    command.action = derivedAction<CameraNode>([&](CameraNode& c, sf::Time) {
        c.splash(sf::Color::Red);
    });
    mCommandQueue.push(command);
}
