#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::hash
{
    class HGetAll : public RedisBaseCmd
    {
    public:
        HGetAll(const std::string& keyName)
            : keyName(keyName)
        {
    
        }

        const std::map<std::string, std::string>& GetFields() const { return fields; }

        const std::string GetFieldValue(const std::string& key) const
        {
            auto iter = fields.find(key);
            if (iter == fields.end())
            {
                return "";
            }
            return iter->second;
        }
        
        bool IsSuccess() const { return isSuccess; }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        std::map<std::string, std::string> fields;

        bool isSuccess;
    };
    
}


