#include "time_mngr.h"

#include "drogon/HttpAppFramework.h"
#include "plugins/common_info/base_lib.h"

using namespace drogon;
using namespace mngr;

void time_mngr::initAndStart(const Json::Value &config)
{
    auto SetStartDate = config["start_date"].asString();
    
    if (SetStartDate.length() != 0)
    {
        auto now = trantor::Date::now();
        auto setDate = trantor::Date::fromDbStringLocal(SetStartDate);
        diff_second = (setDate.microSecondsSinceEpoch() - now.microSecondsSinceEpoch()) / MICRO_SECONDS_PRE_SEC;
    }
    else
    {
        diff_second = 0;
    }
}

void time_mngr::shutdown() 
{

}


