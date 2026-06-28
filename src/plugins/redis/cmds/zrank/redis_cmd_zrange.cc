#include "redis_cmd_zrange.h"

#include <filesystem>

void redis::cmd::hash::ZRange::ToRequest(drogon::OStringStream& req)
{
    req << "ZRange " << keyName << " " << start << " " << end << " WITHSCORES";
        
}

redis::RedisErrno redis::cmd::hash::ZRange::ParseResponse(const drogon::nosql::RedisResult& result)
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
    scores.reserve(arrData.size() / 2);
    for (int i = 0; i < arrData.size(); i += 2)
    {
        auto UserId = arrData[i].asString();
        auto data = arrData[i + 1];
        int64_t score = 0;
        auto dataType = data.type();
        if (dataType == drogon::nosql::RedisResultType::kString)
        {
            score = std::stoi(data.asString());
        }
        else if (dataType == drogon::nosql::RedisResultType::kInteger)
        {
            score = data.asInteger();
        }
        else
        {
            return setResult(RE_ResultNotMatch); 
        }

        scores.emplace_back(UserId, score);
    }

    return setResult(RE_Succ);
}

///----------------------ZRevRange

void redis::cmd::hash::ZRevRange::ToRequest(drogon::OStringStream& req)
{
    req << "ZRevRange " << keyName << " " << start << " " << end << " WITHSCORES";
}


redis::RedisErrno redis::cmd::hash::ZRevRange::ParseResponse(const drogon::nosql::RedisResult& result)
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
    scores.reserve(arrData.size() / 2);
    for (int i = 0; i < arrData.size(); i += 2)
    {
        auto UserId = arrData[i].asString();
        auto data = arrData[i + 1];
        int64_t score = 0;
        auto dataType = data.type();
        if (dataType == drogon::nosql::RedisResultType::kString)
        {
            score = std::stoi(data.asString());
        }
        else if (dataType == drogon::nosql::RedisResultType::kInteger)
        {
            score = data.asInteger();
        }
        else
        {
            return setResult(RE_ResultNotMatch); 
        }

        scores.emplace_back(UserId, score);
    }

    return setResult(RE_Succ);
}

///----------------------ZRevRank


void redis::cmd::hash::ZRevRank::ToRequestArgs(drogon::OStringStream& req, drogon::OStringStream& arg)
{
    req << "ZREVRANK " << keyName << " %s";

    //将 luascript 转换为 utf-8 编码
    arg <<memberName;
}

redis::RedisErrno redis::cmd::hash::ZRevRank::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kInteger)
    {
        return setResult(RE_ResultNotMatch, result.asString()); 
    }

    this->rank = result.asInteger(); 

    return setResult(RE_Succ);
}

