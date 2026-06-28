#pragma once

#include "comp_player.h"
class DceGetActivityCfg;
class DceGetActivityList;
class DceGetMaterialBagList;
class DceBagItemLst;
class BasePlayer;

class ActivityPlayer : public CompPlayer
{
public:
    ActivityPlayer(BasePlayer* base) : CompPlayer(base){}
    virtual ~ActivityPlayer(){}

public:
    void RegisterHandler() override;
    
private:
    drogon::Task<void> HandleDceGetActivityList(DceGetActivityList* msg);
    drogon::Task<void> HandleDceGetActivityCfg(DceGetActivityCfg* msg);
};




