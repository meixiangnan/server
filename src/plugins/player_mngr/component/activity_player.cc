#include "activity_player.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/proto/extends/ExBaseMsg.pb.h"
#include "plugins/proto/extends/ExItemMsg.pb.h"


void ActivityPlayer::RegisterHandler()
{    
    COMP_HANDLER(DceGetActivityList,ActivityPlayer::HandleDceGetActivityList)
    COMP_HANDLER(DceGetActivityCfg, ActivityPlayer::HandleDceGetActivityCfg)
    
}

drogon::Task<void> ActivityPlayer::HandleDceGetActivityList(DceGetActivityList* msg)
{
    DseGetActivityList response;

    host_player->SendMsg(response);
    
    co_return;
}

drogon::Task<void> ActivityPlayer::HandleDceGetActivityCfg(DceGetActivityCfg* msg)
{
    DseGetActivityCfg response;

    response.set_pb_cfg_version(msg->pb_cfg_version());
    
    host_player->SendMsg(response);
    co_return;
}


