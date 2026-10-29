#pragma once

#include "plugins/redis/cmds/redis_cmd_base.h"

#include <vector>

namespace redis
{
namespace cmd
{

class Del : public RedisBaseCmd
{
public:
    Del() = default;

    void AddKey(const std::string& key)
    {
        if (!key.empty())
        {
            keys.push_back(key);
        }
    }

    bool Empty() const { return keys.empty(); }
    long long GetDeleted() const { return deleted; }

protected:
    void ToRequest(drogon::OStringStream& req) override
    {
        req << "DEL";
        for (const auto& key : keys)
        {
            req << " " << key;
        }
    }

    RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override
    {
        if (result.type() != drogon::nosql::RedisResultType::kInteger)
        {
            return setResult(RE_ResultNotMatch);
        }
        deleted = result.asInteger();
        return setResult(RE_Succ);
    }

private:
    std::vector<std::string> keys;
    long long deleted = 0;
};

}
}
