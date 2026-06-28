#pragma once
#include "player.h"

class GamePlayer : public BasePlayer
{
  public:
    GamePlayer()
    {
        
    }

    void InitComps();

    drogon::Task<int> OnLoginLoad();
};
