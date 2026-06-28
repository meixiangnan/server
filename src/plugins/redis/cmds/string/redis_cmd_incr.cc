#include "redis_cmd_incr.h"


void redis::cmd::string::Incr::ToRequest(drogon::OStringStream& req)
{
    req << "Incr " << keyName;
}

redis::RedisErrno redis::cmd::string::Incr::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kInteger)
    {
        return setResult(RE_ResultNotMatch); 
    }

    this->Value  = result.asInteger();
    return setResult(RE_Succ);
}

