#include "redis_cmd_exists.h"

void redis::cmd::string::Exists::ToRequest(drogon::OStringStream& req)
{
    req << "Exists " << keyName;
}

redis::RedisErrno redis::cmd::string::Exists::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kInteger)
    {
        return setResult(redis::RE_ResultNotMatch); 
    }

    auto reply = result.asInteger();
    exists = (reply == Succ);

    return setResult(RE_Succ);
}

