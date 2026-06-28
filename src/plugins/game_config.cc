/**
 *
 *  game_config.cc
 *
 */

#include "game_config.h"

//#include "lua_config/lua_config_accessory.h"

using namespace drogon;
using namespace game;

void config::initAndStart(const Json::Value &config)
{
    this->InitRtBaseCfg();
    
    
}

void config::shutdown() 
{
    /// Shutdown the plugin
}


void config::InitRtBaseCfg()
{

    rtConf.Init();
}



