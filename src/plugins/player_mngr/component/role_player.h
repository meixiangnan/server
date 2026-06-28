#pragma once

#include "comp_player.h"

class DceGetServerTime;
class DceEnterServer;
class DceAuthGate;
class BasePlayer;
class DceGetRandom;
class DceAuth;
class CompPlayer;


class RolePlayer : public CompPlayer
{
public:
    RolePlayer(BasePlayer* base) : CompPlayer(base){}
    virtual ~RolePlayer(){};

    

public:

    drogon::Task<int> OnLoginLoad() override;
    
    void RegisterHandler() override;    
    
private:
    drogon::Task<void> HandleDceGetRandom(DceGetRandom* msg);
    drogon::Task<void> HandleDceAuthGate(DceAuthGate* msg);
    drogon::Task<void> HandleDceEnterServer(DceEnterServer* msg);
    drogon::Task<void> HandleDceGetServerTime(DceGetServerTime* msg);
    drogon::Task<void> HandleDceAuth(DceAuth* msg);
};


