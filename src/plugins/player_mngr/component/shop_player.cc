#include "shop_player.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/proto/extends/ExBaseMsg.pb.h"
#include "plugins/proto/extends/ExShopMsg.pb.h"


void ShopPlayer::RegisterHandler()
{    
    COMP_HANDLER(DceGetShopSaleList, ShopPlayer::HandleDceGetShopSaleList)
}


 drogon::Task<void> ShopPlayer::HandleDceGetShopSaleList(DceGetShopSaleList* msg)
{
    DseGetShopSaleList response;
    response.set_pb_shop_id(msg->pb_shop_id());

    host_player->SendMsg(response);
    co_return;
}
