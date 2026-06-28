#include "redis_cmd_scriptload.h"

#include <drogon/utils/Utilities.h>

void redis::cmd::script::ScriptLoad::ToRequest(drogon::OStringStream& req)
{
   
}

void redis::cmd::script::ScriptLoad::ToRequestArgs(drogon::OStringStream& req, drogon::OStringStream& arg)
{
    req << "SCRIPT LOAD %s";

    //将 luascript 转换为 utf-8 编码
    arg << luascript;
}

redis::RedisErrno redis::cmd::script::ScriptLoad::ParseResponse(const drogon::nosql::RedisResult& result)
{
    if (result.type() != drogon::nosql::RedisResultType::kString)
    {
        return setResult(RE_ResultNotMatch); 
    }

    sha1 = result.asString();

    return setResult(RE_Succ);
}
    //req << "\r\n";

