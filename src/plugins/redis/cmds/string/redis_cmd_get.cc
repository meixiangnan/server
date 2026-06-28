#include "redis_cmd_get.h"

void redis::cmd::string::Get::ToRequest(drogon::OStringStream& req)
{
    req << "Get " << keyName;
}

redis::RedisErrno redis::cmd::string::Get::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kString)
    {
        return setResult(RE_ResultNotMatch); 
    }

    this->Value  = result.asString();
    return setResult(RE_Succ);
}

