#include "redis_cmd_base.h"

#include "drogon/HttpAppFramework.h"

redis::RedisErrno redis::RedisBaseCmd::SyncExecute()
{
    drogon::nosql::RedisClientPtr redisClient = drogon::app().getRedisClient();

    drogon::OStringStream ss;
    ToRequest(ss);

    return redisClient->execCommandSync([this](const drogon::nosql::RedisResult &r){
                   return this->ParseResponse(r);
               }, ss.str());
}

drogon::Task<redis::RedisErrno> redis::RedisBaseCmd::coExecute()
{
    drogon::nosql::RedisClientPtr redisClient = drogon::app().getRedisClient();

    
    drogon::OStringStream req;
    drogon::OStringStream arg;
    
    
    if (UseArgReq())
    {
        ToRequestArgs(req, arg);
    }
    else
    {
        ToRequest(req);    
    }
    
    try
    {
        auto result =  co_await redisClient->execCommandCoro(req.str(), arg.str().data());
        std::cout << result.getStringForDisplaying() << "\n";
        co_return this->ParseResponse(result);
    }
    catch(const drogon::nosql::RedisException &err)
    {
        std::cout << err.what() << "\n";
        co_return RE_Unknown;
    }
}

