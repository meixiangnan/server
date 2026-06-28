#pragma once

#include "comp_player.h"


class BasePlayer;
class DceCreateCampaign;
class DceGetActivityCfg;


class CampaignPlayer : public CompPlayer
{
public:
    CampaignPlayer(BasePlayer* base) : CompPlayer(base){}
    virtual ~CampaignPlayer(){}

public:
    void RegisterHandler() override;
    
private:
    drogon::Task<void> HandleDceCreateCampaign(DceCreateCampaign* msg);
    drogon::Task<void> HandleDceGetActivityCfg(DceGetActivityCfg* msg);
};