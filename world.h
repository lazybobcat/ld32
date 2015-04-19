#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <set>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <resources/resourceholder.h>
#include <resources/resourceidentifiers.h>
#include <resources/musicplayer.h>
#include <resources/soundplayer.h>
#include <resources/scriptplayer.h>
#include <controllers/playercontroller.h>
#include <controllers/aicontroller.h>
#include <events/category.h>
#include <events/commandqueue.h>
#include <entities/cameranode.h>
#include <entities/spritenode.h>
#include <entities/particlenode.h>
#include <entities/textnode.h>
#include <entities/uihealthpoints.h>
#include <entities/platform.h>
#include <entities/medkit.h>
#include <entities/player.h>
#include <entities/unicorn.h>
#include <entities/unicornpathqueue.h>
#include <entities/zombie.h>
#include <entities/boss.h>

class World : public sf::NonCopyable
{
public:
    World(sf::RenderWindow& window, TextureHolder &textures, FontHolder &fonts, MusicPlayer &music, SoundPlayer &sounds, ScriptPlayer &scripts, PlayerController &player, bool menuworld = false);
    ~World();

    void        update(sf::Time dt);
    void        draw();

    CommandQueue&   getCommandQueue();

    void        shakeCamera();
    void        shakeCameraFor(float sec);
    void        splashBlood();

    Player*     getPlayerEntity() { return mPlayerEntity; }
    unsigned int getScore() { return mPoints; }

    void        addZombie(sf::Vector2f pos);
    void        addBoss(Boss::Bosses type, sf::Vector2f pos);
    void        addMedkit(sf::Vector2f pos);

private:
    void        loadTextures();
    void        buildScene();
    void        buildMenuScene();
    void        handleCollisions();

private:
    enum Layer
    {
        Background,
        Foreground,
        UI,
        LayerCount
    };

private:
    sf::RenderWindow&                   mWindow;
    sf::View                            mWorldView;
    TextureHolder&                      mTextures;
    FontHolder&                         mFonts;
    MusicPlayer&                        mMusic;
    SoundPlayer&                        mSounds;
    ScriptPlayer&                       mScripts;
    PlayerController&                   mPlayer;

    SceneNode                           mSceneGraph;
    std::array<SceneNode*,LayerCount>   mSceneLayers;
    CommandQueue                        mCommandQueue;

    // Player
    Player*                             mPlayerEntity;

    // Enemies
    std::set<Entity*>                   mWaitingCreations;
    unsigned int                        mBossKilled;

    // Controllers
    AIController                        mAIController;
    unsigned int                        mPoints;
    TextNode*                           mPointsText;
};

#endif // WORLD_H
