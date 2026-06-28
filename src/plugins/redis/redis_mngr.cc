#include "redis_mngr.h"

#include "cmds/redis_cmd_base.h"
#include "cmds/script/redis_cmd_evalsha.h"
#include "cmds/script/redis_cmd_scriptload.h"
#include "drogon/HttpAppFramework.h"
#include "plugins/message/redis_structs.h"

using namespace drogon;
using namespace mngr;

void redis_mngr::initAndStart(const Json::Value &config)
{
     
}

void redis_mngr::shutdown() 
{

}

drogon::Task<redis::RedisErrno> redis_mngr::GetUsersInfo(std::map<std::string, UserBaseInfo>& ret,
    const std::vector<std::string>& userIds)
{
    if (ScriptSha1GetUserName.empty())
    {
        auto ret = co_await LoadGetUserScriptSha1();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            co_return ret;
        }
    }
    if (ScriptSha1GetUserName.empty())
    {
        co_return redis::RedisErrno::RE_InternalError;
    }

    auto cmd = redis::cmd::script::EvalSha(ScriptSha1GetUserName);
    for (int i = 0; i < userIds.size(); ++i)
    {
        cmd.AddKey(RedisUserData::MakeKey(userIds[i]));
    }
    cmd.AddArg(RedisUserData::NickName());
    cmd.AddArg(RedisUserData::HeadIcon());
    
    auto err = co_await cmd.coExecute();
    if (err != redis::RedisErrno::RE_Succ)
    {
        co_return err;
    }

    auto rrets = cmd.getRetValues();
    for (size_t i = 0; i < userIds.size(); i++)
    {
        ret[userIds[i]].NickName = rrets[i * 2];
        ret[userIds[i]].HeadIcon = rrets[i * 2 + 1];
    }

    co_return redis::RedisErrno::RE_Succ;
}

//eg. LoadGetUserInfo 脚本
drogon::Task<redis::RedisErrno> redis_mngr::LoadGetUserScriptSha1()
{
    std::string GetUserScriptSha1 =  " local results={}\n"
                                     " for i, key in ipairs(KEYS)\n"
                                     "  do results[i] = redis.call('HMGET', key, ARGV[1], ARGV[2])\n"
                                     " end\n"
                                     " return results";
    
    auto cmd = redis::cmd::script::ScriptLoad(GetUserScriptSha1);
    auto err = co_await cmd.coExecute();
    if (err != redis::RedisErrno::RE_Succ)
    {
        co_return err;
    }

    ScriptSha1GetUserName = cmd.GetSha1();    
    co_return redis::RedisErrno::RE_Succ;
}
