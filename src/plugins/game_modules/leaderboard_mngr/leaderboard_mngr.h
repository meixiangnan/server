/**
 *
 *  mngr_verison_mngr.h
 *
 */

#pragma once

#include <set>
#include <drogon/plugins/Plugin.h>
#include <drogon/utils/coroutine.h>

#include "plugins/message/game_defines.h"

namespace mngr
{

class leaderboard_mngr : public drogon::Plugin<leaderboard_mngr>
{
public:
    leaderboard_mngr() {}

    //Drogon Framework
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

public:
    drogon::Task<void> RefreshCache( const std::vector<std::string>& userIds);

    const UserBaseInfo* GetUserNickName(const std::string& userId);
    void RemoveCacheUserNickName(const std::string& userId);
    
private:
    std::map<std::string, UserBaseInfo> CacheUserNick;
    trantor::Date expireTime;

};

}
