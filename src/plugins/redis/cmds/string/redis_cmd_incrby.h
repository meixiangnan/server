#pragma once

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis
{    
    class CmdIncrby : public RedisBaseCmd
    {
        enum SetNXResult
        {
            KeyExists = 0,
            Succ      = 1,
        };
        
    public:
        CmdIncrby(const std::string& keyName, int value)
            : keyName(keyName), incrAmount(value)
        {
            
        }

        void InitReq(const std::string& key, int value)
        {
            keyName = key;
            incrAmount = value;

            afterIncrValue = 0;
        }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;
        
    private:
        std::string keyName;
        int         incrAmount;

        int  afterIncrValue;
    };
}


