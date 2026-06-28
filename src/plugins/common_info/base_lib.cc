#include "base_lib.h"

#include "trantor/utils/Funcs.h"

int game::CPack::FromNetMsg(trantor::MsgBuffer& buffer)
{
    if (buffer.readableBytes() < sizeof(uint16_t))
    {
        return -1;
    }
    
    auto len = trantor::ntoh64(buffer.peekInt16());
    
    if(len > buffer.readableBytes())
    {//not enough  
        return -1;
    }
    
    buffer.readInt16();
    
    this->seqId = trantor::ntoh64(buffer.readInt32());
    this->cmd   = trantor::ntoh64(buffer.readInt16());;
    this->body  = buffer.read(len - 6);
    
    return 0;
}

int game::CPack::ToNetMsg(trantor::MsgBuffer& buffer)
{
    auto len  = body.length() + 6;
    //buffer.appendInt16(len);
    //lua 解析固定使用了小端解析，不明白原因，为何不作为网络序解析
    //buffer.appendInt32LitterEnd(seqId);
    //buffer.appendInt16LitterEnd(cmd);

    buffer.appendInt32(seqId);
    buffer.appendInt16(cmd);

    buffer.append(body);

    return 0;
}

