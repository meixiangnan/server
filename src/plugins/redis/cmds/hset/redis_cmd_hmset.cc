#include "redis_cmd_hmset.h"

void redis::cmd::hash::HMSet::ToRequest(drogon::OStringStream& req)
{
    req << "HMSet " << keyName ;
    for (auto& field : fields)
    {
        req << " "<< field.first << " " ;
        if (field.second.empty()) 
        {
            req << "\"\"";
        }
        else 
        {
            req << field.second;
        }
    }    
}



redis::RedisErrno redis::cmd::hash::HMSet::ParseResponse(const drogon::nosql::RedisResult& result)
{
    
    if (result.type() != drogon::nosql::RedisResultType::kStatus)
    {
        return setResult(RE_ResultNotMatch); 
    }

    return setResult(RE_Succ);
}

