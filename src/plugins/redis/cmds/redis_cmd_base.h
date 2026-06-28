#pragma once

#include <drogon/utils/coroutine.h>

#include "drogon/nosql/RedisResult.h"
#include "drogon/utils/OStringStream.h"
#include "trantor/utils/MsgBuffer.h"

namespace redis
{
    enum RedisErrno
    {
        RE_InternalError = -2,
        RE_Unknown = -1,
        RE_Succ = 0,
        RE_ResultNotMatch = 1,
        RE_SetnxKeyExists = 2,
        RE_SetnxUnknownResult = 3,
        
    };

    class RedisBaseCmd
    {
    public:
        virtual  ~RedisBaseCmd() = default;
        
        
    public:
        RedisErrno SyncExecute();

        drogon::Task<RedisErrno> coExecute();

        
        
    protected:
        virtual void ToRequest(drogon::OStringStream& req) = 0;        
        virtual void ToRequestArgs(drogon::OStringStream& req, drogon::OStringStream& arg) {};

        virtual bool UseArgReq() { return false; }
        virtual RedisErrno ParseResponse(const drogon::nosql::RedisResult& result) = 0;
        
        RedisErrno setResult(RedisErrno err, const std::string & resMsg = "")
        {
            this->resCode = err;
            return this->resCode;
        }
        
        RedisErrno  resCode = RedisErrno::RE_Succ;
        std::string resMsg;
    };
    
}


