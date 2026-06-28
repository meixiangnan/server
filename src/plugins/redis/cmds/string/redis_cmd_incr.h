#pragma once

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"
#include "trantor/utils/MsgBuffer.h"

namespace redis::cmd::string
{
    class Incr : public RedisBaseCmd
    {
    public:
        Incr(const std::string& keyName)
            : keyName(keyName)
        {
    
        }

        int64_t getValue() const { return Value; }
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;

        int64_t Value;
    };
    
}


