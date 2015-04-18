#include <controllers/playercontroller.h>
#include <events/commandqueue.h>
#include <entities/player.h>

PlayerController::PlayerController()
{
    // Default keybindings
    mKeyBinding[sf::Keyboard::Left] = MoveLeft;
    mKeyBinding[sf::Keyboard::Q] = MoveLeft;
    mKeyBinding[sf::Keyboard::A] = MoveLeft;
    mKeyBinding[sf::Keyboard::Right] = MoveRight;
    mKeyBinding[sf::Keyboard::D] = MoveRight;
    mKeyBinding[sf::Keyboard::E] = MoveRight;
    mKeyBinding[sf::Keyboard::Up] = Jump;
    mKeyBinding[sf::Keyboard::Z] = Jump;
    mKeyBinding[sf::Keyboard::W] = Jump;
    mKeyBinding[sf::Keyboard::Space] = Jump;

    // initialize actions
    initializeActions();
}

void PlayerController::handleEvent(const sf::Event &event, CommandQueue &commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        // Check if pressed key appears in key binding, trigger command if so
        auto found = mKeyBinding.find(event.key.code);
        if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
            commands.push(mActionBinding[found->second]);
    }
}

void PlayerController::handleRealtimeInput(CommandQueue &commands)
{
    for(auto pair : mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        {
            commands.push(mActionBinding[pair.second]);
        }
    }
}

void PlayerController::assignKey(Action action, sf::Keyboard::Key key)
{
    // Remove all keys that already map to action
    for(auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
    {
        if (itr->second == action)
            mKeyBinding.erase(itr++);
        else
            ++itr;
    }

    // Insert new binding
    mKeyBinding[key] = action;
}

sf::Keyboard::Key PlayerController::getAssignedKey(Action action) const
{
    for(auto pair : mKeyBinding)
    {
        if (pair.second == action)
            return pair.first;
    }

    return sf::Keyboard::Unknown;
}

void PlayerController::initializeActions()
{
    mActionBinding[MoveLeft].action = derivedAction< Player >([](Player& player, sf::Time) {
        player.move(Entity::Left);
    });
    mActionBinding[MoveLeft].category = Category::Player;

    mActionBinding[MoveRight].action = derivedAction< Player >([](Player& player, sf::Time) {
        player.move(Entity::Right);
    });
    mActionBinding[MoveRight].category = Category::Player;

    mActionBinding[Jump].action = derivedAction< Player >([](Player& player, sf::Time) {
        player.jump();
    });
    mActionBinding[Jump].category = Category::Player;

    // ...
}

bool PlayerController::isRealtimeAction(Action action)
{
    switch (action)
    {
        case MoveLeft:
        case MoveRight:
            return true;

        default:
            return false;
    }
}

