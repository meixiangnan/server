#pragma once

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"
#include "trantor/utils/MsgBuffer.h"

namespace redis::cmd::string
{
    class Exists : public RedisBaseCmd
    {
        enum
        {
            NoExsits = 0,
            Succ     = 1,
        };

    public:
        Exists(const std::string& keyName)
            : keyName(keyName)
        {
    
        }

    public:
        bool GetExists() const { return exists; }

    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        bool exists = false;

    };
}


