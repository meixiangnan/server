#include "game_player.h"

#include "component/activity_player.h"
#include "component/role_player.h"
#include "component/shop_player.h"

void GamePlayer::InitComps()
{
    this->Compoments.AddComp<RolePlayer>(this);
    this->Compoments.AddComp<ShopPlayer>(this);
    this->Compoments.AddComp<ActivityPlayer>(this);
    

    this->Compoments.Foreach([](CompPlayer* comp_player)
    {
        comp_player->RegisterHandler(); 
    });
}

drogon::Task<int> GamePlayer::OnLoginLoad()
{
    Compoments.ForeachCo([](CompPlayer* comp_player)-> drogon::Task<int>
    {
       auto ret = co_await comp_player->OnLoginLoad();
       co_return ret;
    });
    
    co_return 0;
}
