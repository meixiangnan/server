
#include "drogon/HttpController.h"

using namespace drogon;

class GameVerionCtrl : public HttpController<GameVerionCtrl>
{
    void GetVersion(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void GetVsnId  (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void PhoneInfo (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void LogFlag   (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    
  public:
    
    METHOD_LIST_BEGIN
    // list path definitions here;

    //METHOD_ADD  宏会在路径映射中自动把名字空间和类名作为路径的前缀
    ADD_METHOD_TO(GameVerionCtrl::GetVersion, "/user/version/get_version", Post);
    ADD_METHOD_TO(GameVerionCtrl::GetVsnId  , "/user/version/get_vsn_id",  Post);
    ADD_METHOD_TO(GameVerionCtrl::PhoneInfo , "/user/version/phone_info",  Post);
    ADD_METHOD_TO(GameVerionCtrl::LogFlag   , "/user/version/log_flag",    Post);
    
    
    METHOD_LIST_END


    
};