#include "redis_cmd_hmget.h"

void redis::cmd::hash::HMGet::ToRequest(drogon::OStringStream& req)
{
    req << "HMGet " << keyName;
    for (auto& field : fields)
    {
        req << " " << field.first;
    }    
}

redis::RedisErrno redis::cmd::hash::HMGet::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kArray)
    {
        return setResult(RE_ResultNotMatch); 
    }

    auto arrData = result.asArray();

    int i = 0;
    for (auto& field : fields)
    {
        auto data = arrData[i];
        ++i;
        auto dataType = data.type();
        if (dataType == drogon::nosql::RedisResultType::kString)
        {
            field.second = data.asString();
        }
        else if (dataType == drogon::nosql::RedisResultType::kInteger)
        {
            field.second = std::to_string(data.asInteger());
        }
        else if(dataType == drogon::nosql::RedisResultType::kNil)
        {
            field.second = "";
        }
        else 
        {
			return setResult(RE_ResultNotMatch);
		}
    }

    return setResult(RE_Succ);
}

