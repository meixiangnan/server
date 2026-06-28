/**
 *
 *  game_config.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>
#include "common_info/rt_base_config.h"
//#include "luacpp/Source/LuaContext.hpp"

namespace game
{

class config : public drogon::Plugin<config>
{
public:
    config() = default;

public: //plugin framework    
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

public://interface
    const RtBaseConfig& RtConf() const { return  rtConf; }
    
private:
    void InitRtBaseCfg();    

private:

    RtBaseConfig rtConf;
    //LuaCpp::LuaContext luaEnv;
};

}
