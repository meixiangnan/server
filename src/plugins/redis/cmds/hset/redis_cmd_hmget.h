#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::hash
{
    class HMGet : public RedisBaseCmd
    {
    public:
        HMGet(const std::string& keyName)
            : keyName(keyName)
        {
    
        }
        
        void AddField(const std::string& field)
        {
            fields[field] = "";
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


