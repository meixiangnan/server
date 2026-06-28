#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::hash
{
    class HMSet : public RedisBaseCmd
    {
    public:
        HMSet(const std::string& keyName)
            : keyName(keyName)
        {
    
        }

        void AddField(const std::string& field, const std::string& value)
        {
            fields[field] = value;
        }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        std::map<std::string, std::string> fields;
    };
    
}


