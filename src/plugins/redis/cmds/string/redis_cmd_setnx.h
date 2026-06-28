#pragma once

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"
#include "trantor/utils/MsgBuffer.h"

namespace redis::cmd::string
{    
    class SetNX : public RedisBaseCmd
    {
        enum SetNXResult
        {
            KeyExists = 0,
            Succ      = 1,
        };
        
    public:
        SetNX(const std::string& keyName, const std::string& value)
            : keyName(keyName), setValue(value)
        {
            
        }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;
        
    private:
        std::string keyName;
        std::string setValue;   
    };

    class SetEX : public RedisBaseCmd
    {
        enum SetNXResult
        {
            KeyExists = 0,
            Succ      = 1,
        };
        
    public:
        SetEX(const std::string& keyName, int ttl, const std::string& value)
            : keyName(keyName), setTTL(ttl), setValue(value)
        {
            
        }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;
        
    private:
        std::string keyName;
        std::string setValue;
        int         setTTL;
    };
}


