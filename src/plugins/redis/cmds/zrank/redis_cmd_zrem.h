#pragma once

#include "plugins/redis/cmds/redis_cmd_base.h"

#include <vector>

namespace redis
{
namespace cmd
{
namespace hash
{

class ZRem : public RedisBaseCmd
{
public:
    explicit ZRem(const std::string& keyName)
        : keyName(keyName)
    {
    }

    void AddMember(const std::string& member)
    {
        if (!member.empty())
        {
            members.push_back(member);
        }
    }

    bool Empty() const { return members.empty(); }

protected:
    void ToRequest(drogon::OStringStream& req) override
    {
        req << "ZREM " << keyName;
        for (const auto& member : members)
        {
            req << " " << member;
        }
    }

    RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override
    {
        if (result.type() != drogon::nosql::RedisResultType::kInteger)
        {
            return setResult(RE_ResultNotMatch);
        }
        return setResult(RE_Succ);
    }

private:
    std::string keyName;
    std::vector<std::string> members;
};

}
}
}
