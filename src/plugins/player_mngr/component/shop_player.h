#pragma once

#include "comp_player.h"
class DceGetShopSaleList;
class BasePlayer;

class ShopPlayer : public CompPlayer
{
public:
    ShopPlayer(BasePlayer* base) : CompPlayer(base){}
    virtual ~ShopPlayer(){}

public:
    void RegisterHandler() override;
    
private:
     drogon::Task<void> HandleDceGetShopSaleList(DceGetShopSaleList* msg);
};




