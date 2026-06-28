#include "redis_cmd_zadd.h"

void redis::cmd::hash::ZAdd::ToRequest(drogon::OStringStream& req)
{
    req << "ZAdd " << keyName ;
    for (auto& field : scores)
    {
        req << " "<< field.second << " " << field.first;
    }    
}

redis::RedisErrno redis::cmd::hash::ZAdd::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kInteger)
    {
        return setResult(RE_ResultNotMatch); 
    }

    return setResult(RE_Succ);
}

