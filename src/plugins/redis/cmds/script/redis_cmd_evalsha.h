#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::script
{
    class EvalSha : public RedisBaseCmd
    {
    public:
        EvalSha(const std::string& sha1)
            : luascript_sha1(sha1)
        {
    
        }

        void AddKeys(const std::vector<std::string>& skeys)
        {
            keys.insert(keys.end(), skeys.begin(), skeys.end());            
        }
        
        void AddArgs(const std::vector<std::string>& sargs)
        {
            args.insert(args.end(), sargs.begin(), sargs.end());
        }
        
        void AddKey(const std::string& key)
        {
            keys.push_back(key);
        }
        
        void AddArg(const std::string& arg)
        {
            args.push_back(arg);
        }

        const std::vector<std::string>& getRetValues() { return retValues; }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string luascript_sha1;
        std::vector<std::string> keys;
        std::vector<std::string> args;

        std::vector<std::string> retValues;
    };
    
}


