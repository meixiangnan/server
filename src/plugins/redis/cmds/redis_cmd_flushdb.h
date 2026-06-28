#pragma once

#include "plugins/redis/cmds/redis_cmd_base.h"

namespace redis
{
namespace cmd
{

class FlushDB : public RedisBaseCmd
{
public:
    FlushDB() = default;
    ~FlushDB() override = default;

protected:
    void ToRequest(drogon::OStringStream& req) override
    {
        req << "FLUSHDB";
    }

    RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override
    {
        if (result.type() != drogon::nosql::RedisResultType::kStatus)
        {
            return setResult(RE_ResultNotMatch);
        }
        return setResult(RE_Succ);
    }
};

}
}
