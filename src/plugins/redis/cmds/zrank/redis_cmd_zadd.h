#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::hash
{
    class ZAdd : public RedisBaseCmd
    {
    public:
        ZAdd(const std::string& keyName)
            : keyName(keyName)
        {
    
        }

        void AddScore(const std::string& who, int64_t score)
        {
            scores[who] = score;
        }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        std::map<std::string, int64_t> scores;
    };
    
}


