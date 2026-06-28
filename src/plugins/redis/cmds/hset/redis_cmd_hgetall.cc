#include "redis_cmd_hgetall.h"

void redis::cmd::hash::HGetAll::ToRequest(drogon::OStringStream& req)
{
    req << "HGetAll " << keyName;
    //req << "\r\n";
}

redis::RedisErrno redis::cmd::hash::HGetAll::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kArray)
    {
        return setResult(RE_ResultNotMatch); 
    }

    auto arrData = result.asArray();
    if (arrData.size() % 2 != 0)
    {
        return setResult(RE_ResultNotMatch); 
    }

    
    for (int i = 0; i < arrData.size(); i += 2)
    {
        auto field = arrData[i].asString();
        auto data = arrData[i + 1];
        auto dataType = data.type();
        if (dataType == drogon::nosql::RedisResultType::kString)
        {
            fields[field] = data.asString();
        }
        else if (dataType == drogon::nosql::RedisResultType::kInteger)
        {
            fields[field] = std::to_string(data.asInteger());
        }
        else
        {
            return setResult(RE_ResultNotMatch); 
        }
    }

    return setResult(RE_Succ);
}

