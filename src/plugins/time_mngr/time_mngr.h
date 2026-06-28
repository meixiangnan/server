#pragma once

#include <drogon/plugins/Plugin.h>

#include "drogon/utils/coroutine.h"

namespace mngr
{   
    class time_mngr : public drogon::Plugin<time_mngr>
    {
    public:
        time_mngr() = default;
        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        void initAndStart(const Json::Value &config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        void shutdown() override;
        
    public:        
        inline trantor::Date GetServerDate() 
        {
			auto now = trantor::Date::now();
            auto str = now.toDbStringLocal();//转换时间为本时区时间，方便计算。
            now = trantor::Date::fromDbString(str);
			return trantor::Date(now.microSecondsSinceEpoch() + (diff_second * MICRO_SECONDS_PRE_SEC));
		}
        
    private:
        int diff_second = 0;
        
    };


}
