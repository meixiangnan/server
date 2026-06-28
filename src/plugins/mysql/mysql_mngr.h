#pragma once

#include <drogon/plugins/Plugin.h>

#include "drogon/utils/coroutine.h"
#include "plugins/proto/proto_sri/RoleItem.h"

namespace mngr
{   
    class mysql_mngr : public drogon::Plugin<mysql_mngr>
    {
    public:
        mysql_mngr() = default;
        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        void initAndStart(const Json::Value &config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        void shutdown() override;



    public :
        drogon::Task<int> LoadRoleItems(std::vector<RoleItem>& items, int64_t pstid);
        
        drogon::Task<int> LoadRoleInfo(RoleItem, int64_t pstid);
        
        
    };


}
