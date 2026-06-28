#include "role_player.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/mysql/mysql_mngr.h"
#include "plugins/proto/extends/ExBaseMsg.pb.h"
#include "plugins/proto/extends/ExLoginMsg.pb.h"


drogon::Task<int> RolePlayer::OnLoginLoad()
{
    co_return 0;
}

void RolePlayer::RegisterHandler()
{
    
    COMP_HANDLER(DceGetRandom, RolePlayer::HandleDceGetRandom)
    COMP_HANDLER(DceAuthGate, RolePlayer::HandleDceAuthGate)
    COMP_HANDLER(DceEnterServer, RolePlayer::HandleDceEnterServer)
    COMP_HANDLER(DceGetServerTime, RolePlayer::HandleDceGetServerTime)
    
    COMP_HANDLER(DceAuth, RolePlayer::HandleDceAuth)
}



drogon::Task<void> RolePlayer::HandleDceGetRandom(DceGetRandom* msg)
{
    DseGetRandom response;
    response.set_pb_random("10");

    host_player->SendMsg(response);
    co_return;
}

drogon::Task<void> RolePlayer::HandleDceAuthGate(DceAuthGate* msg)
{
    DseAuthGate response;
    response.set_pb_auth_result(0);
    response.set_pb_open_id(msg->pb_open_id());
    
    
    host_player->SendMsg(response);
    co_return;
}

drogon::Task<void> RolePlayer::HandleDceEnterServer(DceEnterServer* msg)
{
    DseEnterServer response;
    response.set_pb_game_server_id(1);
    response.set_pb_role_id(1001);
    response.set_pb_result(0);
    response.set_pb_region_id(1);
    
    host_player->SendMsg(response);
    co_return;
}

drogon::Task<void> RolePlayer::HandleDceGetServerTime(DceGetServerTime* msg)
{
    DseGetServerTime response;

    auto cur_tick = trantor::Date::now();
    response.set_pb_server_time_ms(cur_tick.microSecondsSinceEpoch() / 100);

    auto offset = trantor::Date::timezoneOffset();
    
    response.set_pb_server_time_zone(offset);
    host_player->SendMsg(response);
    co_return;
}

drogon::Task<void> RolePlayer::HandleDceAuth(DceAuth* msg)
{
    
    co_return;
}


