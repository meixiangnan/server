#include "player.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/common_info/base_lib.h"
#include "plugins/proto/extends/ExLoginMsg.pb.h"

using namespace drogon;

BasePlayer::BasePlayer()
    :last_online_tick()
{
    
}

BasePlayer::~BasePlayer()
{
    for (auto pair : handlers)
    {
        delete pair.second;
    }
}

void BasePlayer::Online()
{
    struct tm tmptm;
    auto epoch = timegm(&tmptm);
    
    last_online_tick = trantor::Date(epoch * MICRO_SECONDS_PRE_SEC);
}

void BasePlayer::SetAuth()
{
    is_authorized = true;
}

bool BasePlayer::RecvSeq(int seq)
{
    if(this->last_seq >= seq)
    {
        return false;
    }

    if(this->last_seq + 1 < seq)
    {
        //need queue this pack;
        return false;
    }

    this->last_seq = seq;
    return true;    
}

void BasePlayer::SetRandomSalt(int salt)
{
    random_salt = salt;
}

void BasePlayer::BindConnection(drogon::WebSocketConnectionPtr conn)
{
    Connection = conn;
}

Task<void> BasePlayer::HandleMsg(int cmd_id, google::protobuf::Message* msg)
{
    auto iter = this->handlers.find(cmd_id);
    if (iter == this->handlers.end())
    {
        co_return;        
    }
    co_await iter->second->HandleMsg(msg);
}

int BasePlayer::SendMsg(const google::protobuf::Message& msg)
{
    auto ProtoRegistry = app().getPlugin<game::ProtoRegistry>();
    auto cmd_id = ProtoRegistry->GetCmdIdOfMessage(&msg);
    if (-1 == cmd_id)
    {
        return -1;
    }
    
    
    game::CPack response;
    response.seqId = this->last_seq;
    response.cmd = static_cast<short>(cmd_id);
    msg.SerializeToString(&response.body);

    trantor::MsgBuffer buffer;
    response.ToNetMsg(buffer);

    this->Connection->send(buffer.peek(), buffer.readableBytes(), WebSocketMessageType::Binary);
    return 0;
}