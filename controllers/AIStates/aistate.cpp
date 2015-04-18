#include "aistate.h"
#include <entities/player.h>
#include <entities/zombie.h>

AIState::AIState(Player &player) :
    mPlayer(player)
{
}
