#pragma once

#include <drogon/plugins/Plugin.h>

#include "drogon/HttpAppFramework.h"
#include "drogon/WebSocketConnection.h"
#include "plugins/common_info/base_lib.h"
#include "plugins/common_info/compoment.h"
#include "plugins/proto/proto_mngr.h"
#include "plugins/proto/extends/ExMessageMsg.pb.h"
#include "trantor/utils/MsgBuffer.h"

class DceGetRandom;
class DceAuth;
class CompPlayer;

typedef std::function<drogon::Task<void>(google::protobuf::Message*)> HandleFunc;

class IMsgHandler
{
public:
    virtual drogon::Task<void> HandleMsg(google::protobuf::Message* msg) = 0; 
};

template<typename TMsg>
class MsgHandler : public IMsgHandler
{
    typedef std::function<drogon::Task<void>(TMsg*)> TMsgHandler;
public:
    explicit  MsgHandler( TMsgHandler impl)
    : HandleImpl(impl)
    {
        
    }

    virtual drogon::Task<void> HandleMsg(google::protobuf::Message* msg) override 
    {
        TMsg* tMsg = dynamic_cast<TMsg*>(msg);
        if (nullptr == tMsg)
        {
            co_return;
        }

        co_await this->HandleImpl(tMsg);
    }
    
private:
    TMsgHandler HandleImpl;
};


class BasePlayer
{
public:
    BasePlayer();

    virtual ~BasePlayer();
    
public:
    virtual void Online();
    virtual void SetAuth();
    virtual bool RecvSeq(int seq);
    virtual void SetRandomSalt(int salt);
    virtual void BindConnection(drogon::WebSocketConnectionPtr conn);

    drogon::Task<void> HandleMsg(int cmd_id, google::protobuf::Message* msg);
    virtual int  SendMsg(const google::protobuf::Message& msg);

    template<typename TMsg>
    void Register(IMsgHandler* handler)
    {
        auto ProtoRegistry = drogon::app().getPlugin<game::ProtoRegistry>();
        auto cmd_id = ProtoRegistry->GetCmdIdOfMessage<TMsg>();
        if (cmd_id == -1)
        {
            return;
        }

        auto iter = this->handlers.find(cmd_id); 
        if (iter != this->handlers.end())
        {
            delete iter->second;            
        }
        
        this->handlers[cmd_id] = handler;
    }

    template<typename TComp>
    CompPlayer* Get() { return Compoments.Get<TComp>(); }

    int64_t GetPlayerId()const { return player_id; }
    
public:
    trantor::MsgBuffer buffer;
    drogon::WebSocketConnectionPtr Connection;
    CompCenter<CompPlayer> Compoments;
    
    
protected:
    trantor::Date last_online_tick;

    int  last_seq = 0;
    bool is_authorized = false;
    int  random_salt = 0;
    int64_t player_id = 0;


    std::map<int, IMsgHandler*> handlers;
    
};
