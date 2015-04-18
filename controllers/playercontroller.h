#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <events/command.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <vector>

class CommandQueue;

class PlayerController : private sf::NonCopyable
{
public:
    enum Action
    {
        // Player Actions here...
        MoveLeft,
        MoveRight,
        Jump,
        FirePath,
        FirePathClear,
        Fire,
        // Fire ? LaunchMissile ? Jump ? ...
        ActionCount
    };

public:
    PlayerController(sf::RenderWindow& window);

    void                handleEvent(const sf::Event& event, CommandQueue& commands);
    void                handleRealtimeInput(CommandQueue& commands);

    void                assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key   getAssignedKey(Action action) const;

private:
    void                initializeActions();
    static bool         isRealtimeAction(Action action);

private:
    sf::RenderWindow&                       mWindow;
    std::map<sf::Keyboard::Key, Action>		mKeyBinding;
    std::map<Action, Command>				mActionBinding;

    std::vector<sf::Vector2f>               mFirePath;
};

#endif // PLAYER_CONTROLLER_H
