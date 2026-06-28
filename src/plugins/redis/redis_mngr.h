/**
 *
 *  mngr_verison_mngr.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>

#include "cmds/redis_cmd_base.h"
#include "plugins/message/game_defines.h"

namespace redis
{
    class RedisBaseCmd;
}

namespace mngr
{
    
class redis_mngr : public drogon::Plugin<redis_mngr>
{    
public:
    
    redis_mngr() = default;
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;   

    
    drogon::Task<redis::RedisErrno> GetUsersInfo(std::map<std::string, UserBaseInfo>& ret, const std::vector<std::string>& userIds);

    
private:
    drogon::Task<redis::RedisErrno> LoadGetUserScriptSha1();
    
    std::string ScriptSha1GetUserName = "";
};


}
