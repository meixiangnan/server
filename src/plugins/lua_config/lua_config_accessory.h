
#include <string>

#include "luacpp/Source/LuaContext.hpp"
#include "luacpp/Source/Engine/LuaTTable.hpp"


class LuaConfigParser
{
public:
    void Init(LuaCpp::LuaContext LuaEnv, const std::string& _LuaConfigPath);

    int FindData(LuaCpp::Engine::LuaTTable& ret, int configId);

    std::unique_ptr<LuaCpp::Engine::LuaState> DataState = nullptr;

private:
    void InitUniqueName(const std::string& _LuaConfigPath);
private:
    std::string luaConfigPath;
    std::string luaConfigUniqueName;
};


class Accessory
{
public:
    int ItemId      = 0;
    int ItemType    = 0; 
    int CanPile     = 0;
    int Quality     = 0;    
    int bagType     = 0;
    int itemInitNum = 0;
    int SetId       = 0;
    int PlanId      = 0;

    std::string AttrPackage = "";
};

class LuaConfigAccessory
{
public:
    void Init(LuaCpp::LuaContext LuaEnv);

    bool FindData(Accessory& ret, int ItemId);

private:
    LuaConfigParser config_parser_;

    std::map<int, Accessory> data_cache;
};



