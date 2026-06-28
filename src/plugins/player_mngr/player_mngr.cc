/**
 *
 *  mngr_verison_mngr.cc
 *
 */

#include "player_mngr.h"

#include "game_player.h"
#include "player.h"

using namespace drogon;
using namespace mngr;

void PlayerMngr::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
}

void PlayerMngr::shutdown() 
{
    /// Shutdown the plugin
}

std::shared_ptr<GamePlayer> PlayerMngr::CreatePlayer()
{
    auto player = std::make_shared<GamePlayer>();

    player->InitComps();
    
    wait_auth_players.insert(player);

    return player;
}

bool PlayerMngr::AuthPlayer(int account_id, std::shared_ptr<GamePlayer> player)
{
    auto it = wait_auth_players.find(player);
    if (it == wait_auth_players.end())
    {
        return false;
    }

    player->SetAuth();
    wait_auth_players.erase(it);

    auto onlineIter = players.find(account_id);
    if (onlineIter == players.end())
    {
        //kick Player;        
    }
    
    players[account_id] = player;
    return true;
}
