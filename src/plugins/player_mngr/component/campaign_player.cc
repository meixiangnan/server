#include "campaign_player.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/proto/extends/ExBaseMsg.pb.h"
#include "plugins/proto/extends/ExItemMsg.pb.h"
#include "plugins/proto/extends/ExPlayerMsg.pb.h"


void CampaignPlayer::RegisterHandler()
{    
    COMP_HANDLER(DceCreateCampaign,CampaignPlayer::HandleDceCreateCampaign)
    
}

drogon::Task<void> CampaignPlayer::HandleDceCreateCampaign(DceCreateCampaign* msg)
{
    DseCreateCampaign response;
    response.set_pb_campaign_id(msg->pb_campaign_id());
    //response.set_pb_level_name();

    host_player->SendMsg(response);
    co_return;
}

drogon::Task<void> CampaignPlayer::HandleDceGetActivityCfg(DceGetActivityCfg* msg)
{
    DseGetActivityCfg response;
    

    host_player->SendMsg(response);
    co_return;
}



