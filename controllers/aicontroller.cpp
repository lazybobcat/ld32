#include "aicontroller.h"

AIController::AIController() :
    mPlayer(nullptr)
{
}


void AIController::update(sf::Time dt, CommandQueue &commands, Zombie &zombie)
{
    if(!mPlayer)
    {
        Command command;
        command.category = Category::Player;
        command.action = derivedAction<Player>([this] (Player& player, sf::Time) {
            mPlayer = &player;
        });
        commands.push(command);

        return;
    }

    if(!zombie.mCurrentBehavior)
    {
        zombie.setBehavior(new WaitingBehavior(*mPlayer, 1.2f));
    }
}
