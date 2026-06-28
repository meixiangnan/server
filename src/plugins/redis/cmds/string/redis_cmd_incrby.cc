#include "redis_cmd_incrby.h"
#include "drogon/utils/OStringStream.h"

void redis::CmdIncrby::ToRequest(drogon::OStringStream& req)
{
    req << "INCRBY " << keyName << " " << incrAmount ;
}

redis::RedisErrno redis::CmdIncrby::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kInteger)
    {
        return setResult(RE_ResultNotMatch, result.asString()); 
    }

    this->afterIncrValue = result.asInteger(); 

    return setResult(RE_Succ);
}

