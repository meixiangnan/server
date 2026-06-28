#pragma once

#include <map>

#include "../redis_cmd_base.h"
#include "drogon/nosql/RedisResult.h"

namespace redis::cmd::hash
{
    class ZRange : public RedisBaseCmd
    {
    public:
        ZRange(const std::string& keyName, int64_t start, int64_t end)
            : keyName(keyName), start(start), end(end)
        {
    
        }

        //获取结果
        const std::vector<std::pair<std::string, int64_t>>& getResult() const { return scores; }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        int64_t start;
        int64_t end;
        
        std::vector<std::pair<std::string, int64_t>> scores;
    };

    class ZRevRange : public RedisBaseCmd
    {
    public:
        ZRevRange(const std::string& keyName, int64_t start, int64_t end)
            : keyName(keyName), start(start), end(end)
        {
    
        }

        //获取结果
        const std::vector<std::pair<std::string, int64_t>>& getResult() const { return scores; }
        
    protected: 
        //impl BaseRedisCmd
        void       ToRequest(drogon::OStringStream& req) override;
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        int64_t start;
        int64_t end;
        
        std::vector<std::pair<std::string, int64_t>> scores;
    };
    

    class ZRevRank : public RedisBaseCmd
    {
    public:
        ZRevRank(const std::string& keyName, const std::string& memberName)
            : keyName(keyName), memberName(memberName)
        {
    
        }

        //获取结果
        int getRank() const { return rank;}
        
    protected: 
        //impl BaseRedisCmd
        virtual void ToRequest(drogon::OStringStream& req){}
        void ToRequestArgs(drogon::OStringStream& req, drogon::OStringStream& arg) override;

        virtual bool UseArgReq() { return true; }
        RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) override;

    private:
        std::string keyName;
        std::string memberName;
        
        int rank;
    };
    
}


