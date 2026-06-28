#include "mysql_mngr.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/common_info/base_lib.h"
#include "plugins/proto/proto_sri/RoleItem.h"

using namespace drogon;
using namespace mngr;

void mysql_mngr::initAndStart(const Json::Value &config)
{
     
}

void mysql_mngr::shutdown() 
{

}

drogon::Task<int> mysql_mngr::LoadRoleItems(std::vector<RoleItem>& items, int64_t pstid)
{
    auto loadSql = RoleItem::GetLoadAllSql() + " where pstid = " + std::to_string(pstid);
    auto result = co_await drogon::app().getDbClient()->execSqlCoro(loadSql);
        
    if (result.size() == 0)
    {
        co_return 0;
    }  
    //使用result填充RoleItem
    for (auto row : result)
    {
        RoleItem item;
        item.UnSerializeFromDb(row);        
        items.push_back(item);
    }

    co_return 0;
}


