#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::script
{
    class ScriptLoad : public RedisBaseCmd
    {
    public:
        ScriptLoad(const std::string& luascript)
            : luascript(luascript)
        {
    
        }
        const std::string& GetSha1() const { return sha1; }
        
        bool IsSuccess() const { return isSuccess; }
        
    protected: 
        //impl BaseRedisCmd
        void        ToRequest(drogon::OStringStream& req) override;
        bool        UseArgReq() override { return true; }
        
        void        ToRequestArgs(drogon::OStringStream& req, drogon::OStringStream& arg) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string luascript;
        std::string sha1;

        bool isSuccess;
    };
    
}


