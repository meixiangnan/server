/**
 *
 *  game_config.h
 *
 */

#pragma once

#include "trantor/utils/MsgBuffer.h"

namespace game
{ 
    class CPack
    {
    public:
        //应该拆分包体和包头
        int   seqId;
        short cmd;
        
        std::string body;

    public:
        int FromNetMsg(trantor::MsgBuffer& buffer);
        
        int ToNetMsg(trantor::MsgBuffer& buffer);  
    };
    
    
  
    
}

#include <sstream>
#include <trantor/utils/Date.h>

namespace trantor
{
    static constexpr size_t kBufferOffset{8};


    inline std::stringstream& operator<<(std::stringstream& os, const MsgBuffer& buf)
    {
        std::string s(buf.peek(), buf.readableBytes());        
        os << s;
        return os;
    }

    inline std::stringstream& operator>>(std::stringstream& is, MsgBuffer& buf)
    {
        buf.append(is.str());
        return is;
    }

    inline std::ostream& operator<<(std::ostream& os, const Date& date)
    {
        os << date.microSecondsSinceEpoch();
        return os;
    }

    inline std::istream& operator>>(std::istream& is, Date& date)
    {
        int64_t microSeconds;
        is >> microSeconds;
        date = Date(microSeconds);
        return is;
    }
}