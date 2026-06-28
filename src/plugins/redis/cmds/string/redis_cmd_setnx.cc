#include "redis_cmd_setnx.h"

void redis::cmd::string::SetNX::ToRequest(drogon::OStringStream& req)
{
    req << "SetNx " << keyName << " " << setValue;
}

redis::RedisErrno redis::cmd::string::SetNX::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kInteger)
    {
        return setResult(RE_ResultNotMatch); 
    }

    auto reply = result.asInteger();
    if (reply == SetNXResult::KeyExists)
    {
        return setResult(RE_SetnxKeyExists);
    }
    
    if (reply != SetNXResult::Succ)
    {
        return setResult(RE_SetnxUnknownResult);    
    }

    return setResult(RE_Succ);
}


void redis::cmd::string::SetEX::ToRequest(drogon::OStringStream& req)
{
    req << "SetEx " << keyName << " " << setTTL << " " << setValue;
}

redis::RedisErrno redis::cmd::string::SetEX::ParseResponse(const drogon::nosql::RedisResult& result)
{
    return setResult(RE_Succ);
}

