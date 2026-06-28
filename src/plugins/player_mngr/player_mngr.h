/**
 *
 *  mngr_verison_mngr.h
 *
 */

#pragma once

#include <set>
#include <drogon/plugins/Plugin.h>
class GamePlayer;
class BasePlayer;

namespace mngr
{

class PlayerMngr : public drogon::Plugin<PlayerMngr>
{
  public:
    PlayerMngr() {}

    //Drogon Framework
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

public:

    std::shared_ptr<GamePlayer> CreatePlayer();

    bool AuthPlayer(int account_id, std::shared_ptr<GamePlayer> player);

private:

    std::map<int, std::shared_ptr<GamePlayer>> players;

    std::set<std::shared_ptr<GamePlayer>> wait_auth_players;
};

}
