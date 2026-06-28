#pragma once

#include "plugins/common_info/compoment.h"
#include "plugins/player_mngr/player.h"
#include "plugins/proto/extends/ExMessageMsg.pb.h"


class CompPlayer : public IComponent<CompPlayer>
{
public:
    CompPlayer(BasePlayer* host)
        : host_player(host)
        , IComponent(&(host->Compoments))
    {
        
    }
    virtual ~CompPlayer() {};
    

public://框架生命周期接口
    virtual void RegisterHandler() = 0;

    virtual drogon::Task<int> OnLoginLoad()
    {
        co_return 0;  
    };


protected:    
#define COMP_HANDLER(TMsg, FUNCName) \
{auto handler = new MsgHandler<TMsg>(std::bind(&FUNCName, this, std::placeholders::_1));\
host_player->Register<TMsg>(handler);}
    
    BasePlayer* host_player;
};

