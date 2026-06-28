
#include "lua_config_accessory.h"

#include "luacpp/Source/Engine/LuaTNumber.hpp"
#include "luacpp/Source/Engine/LuaTString.hpp"
#include "luacpp/Source/Engine/LuaTTable.hpp"

void LuaConfigParser::Init(LuaCpp::LuaContext luaEnv, const std::string& _LuaConfigPath)
{
    this->luaConfigPath = _LuaConfigPath;
    this->InitUniqueName(_LuaConfigPath);
    
    luaEnv.CompileFile(this->luaConfigUniqueName, this->luaConfigPath);
    this->DataState = luaEnv.newStateFor(this->luaConfigUniqueName);
    lua_pcall(*DataState, 0, LUA_MULTRET, 0);// 脚本执行结束，堆栈中唯一的table就是返回值  
}

int LuaConfigParser::FindData(LuaCpp::Engine::LuaTTable& ret, int configId)
{
    int retCode = 0;
    try
    {
        LuaCpp::Engine::LuaTTable dataTbl;
        dataTbl.PopValue(*DataState);
    
        LuaCpp::Engine::LuaTString GetFuncName("GetDataByKey");
        GetFuncName.PushValue(*DataState);
    
        lua_gettable(*DataState, -2);//从table中查询 GetDataByKey 方法  == table["GetDataByKey"] == tab.GetDataByKey
        lua_pushvalue(*DataState, 1); //将当前栈低的table，作为方法的参数放到方法第一个tab.GetDataByKey(tab,

        LuaCpp::Engine::LuaTNumber selIndex(configId);
        selIndex.PushValue(*DataState);//第二个参数 tab.GetDataByKey(tab,42030)

        lua_pcall(*DataState, 2, 1, 0); //执行tab.GetDataByKey(tab,42030) 栈顶为当前的查询返回值

        ret.PopValue(*DataState);        
    }
    catch (...)
    {
        retCode = -1;
    }
    
    lua_settop(*DataState , 1);//保持栈顶始终只有配置这个table
    return retCode;
}

void LuaConfigParser::InitUniqueName(const std::string& _LuaConfigPath)
{
    auto pos = _LuaConfigPath.find_last_not_of("/");
    if (pos == std::string::npos)
    {
        this->luaConfigUniqueName = _LuaConfigPath;
        return;
    }

    this->luaConfigUniqueName = _LuaConfigPath.substr(pos + 1);
    
}

void LuaConfigAccessory::Init(LuaCpp::LuaContext luaEnv)
{
    config_parser_.Init(luaEnv, "../config/lua/server/config/ConfigAccessory.lua");
}

bool LuaConfigAccessory::FindData(Accessory& ret, int ItemId)
{
    auto iter = this->data_cache.find(ItemId); 
    if( iter != this->data_cache.end())
    {
        ret = iter->second;
        return true;
    }

    LuaCpp::Engine::LuaTTable dataTbl;
    if(0 != this->config_parser_.FindData(dataTbl, ItemId))
    {
        return false;
    }

    auto& itemType = dataTbl.getValue(LuaCpp::Engine::Table::Key("item_type"));

    ret.ItemType = dynamic_cast<LuaCpp::Engine::LuaTNumber&>(itemType).getValue(); 
}
