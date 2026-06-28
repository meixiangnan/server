/**
 *
 *  mngr_verison_mngr.cc
 *
 */

#include "leaderboard_mngr.h"

#include <drogon/HttpAppFramework.h>

#include "plugins/redis/redis_mngr.h"

using namespace drogon;
using namespace mngr;

void leaderboard_mngr::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
}

void leaderboard_mngr::shutdown() 
{
    /// Shutdown the plugin
}


drogon::Task<void> leaderboard_mngr::RefreshCache(const std::vector<std::string>& userIds)
{
    std::vector<std::string> NeedFreshIds;
    for(auto& userId : userIds)
    {
        auto it = CacheUserNick.find(userId);
        if(it == CacheUserNick.end())
        {
            NeedFreshIds.push_back(userId);
        }
    }

    std::map<std::string, UserBaseInfo> NewNicks;
    redis::RedisErrno ret = co_await app().getPlugin<redis_mngr>()->GetUsersInfo(NewNicks, NeedFreshIds);
    if(ret != redis::RedisErrno::RE_Succ)
    {
        co_return;
    }
    for(auto& it : NewNicks)
    {
        CacheUserNick[it.first] = it.second;
    }
}

const UserBaseInfo* leaderboard_mngr::GetUserNickName(const std::string& userId)
{
    auto it = CacheUserNick.find(userId);
    if(it == CacheUserNick.end())
    {
        return nullptr;
    }
    return &(it->second);
}

void leaderboard_mngr::RemoveCacheUserNickName(const std::string& userId)
{
    auto it = CacheUserNick.find(userId);
    if(it == CacheUserNick.end())
    {
        return;
    }
    CacheUserNick.erase(it);
}
