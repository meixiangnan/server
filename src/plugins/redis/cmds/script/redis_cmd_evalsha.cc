#include "redis_cmd_evalsha.h"


void redis::cmd::script::EvalSha::ToRequest(drogon::OStringStream& req)
{
    req << "EVALSHA " << luascript_sha1 << " " << keys.size();
    for (const auto& key : keys)
    {
        req << " " << key;
    }
    for (const auto& arg : args)
    {
        req << " " << arg;
    }
}

redis::RedisErrno redis::cmd::script::EvalSha::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kArray)
    {
        return setResult(RE_ResultNotMatch); 
    }

    auto array = result.asArray();
    if (array.empty())
    {
        return setResult(RE_ResultNotMatch); 
    }

    for (const auto& itembundle : array)
    {
        for (const auto& item : itembundle.asArray())
        {
            retValues.push_back(item.asString());    
        }
        
    }

    return setResult(RE_Succ);
}
    //req << "\r\n";

