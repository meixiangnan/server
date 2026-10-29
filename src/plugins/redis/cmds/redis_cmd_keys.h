#pragma once

#include "plugins/redis/cmds/redis_cmd_base.h"

#include <vector>

namespace redis
{
namespace cmd
{

class Keys : public RedisBaseCmd
{
public:
    explicit Keys(const std::string& pattern)
        : pattern(pattern)
    {
    }

    const std::vector<std::string>& GetKeys() const { return keys; }

protected:
    void ToRequest(drogon::OStringStream& req) override
    {
        req << "KEYS " << pattern;
    }

    RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override
    {
        if (result.type() == drogon::nosql::RedisResultType::kNil)
        {
            return setResult(RE_Succ);
        }

        if (result.type() != drogon::nosql::RedisResultType::kArray)
        {
            return setResult(RE_ResultNotMatch);
        }

        auto arrData = result.asArray();
        keys.reserve(arrData.size());
        for (const auto& item : arrData)
        {
            if (item.type() == drogon::nosql::RedisResultType::kString)
            {
                keys.push_back(item.asString());
            }
        }
        return setResult(RE_Succ);
    }

private:
    std::string pattern;
    std::vector<std::string> keys;
};

}
}
