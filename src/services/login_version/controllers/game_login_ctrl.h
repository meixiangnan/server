
#include "drogon/HttpController.h"

using namespace drogon;

class GameLoginCtrl : public HttpController<GameLoginCtrl>
{
    void get_auth         (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void get_server_notice(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void refresh_token    (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    
    void create_role      (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void get_server_list  (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);    
    void pre_create_role  (const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
       
    
  public:
    
    METHOD_LIST_BEGIN
    // list path definitions here;

    //METHOD_ADD  宏会在路径映射中自动把名字空间和类名作为路径的前缀
    ADD_METHOD_TO(GameLoginCtrl::get_auth,         "/user/login/pre/get_auth",           Post);
    ADD_METHOD_TO(GameLoginCtrl::get_server_notice,"/user/login/pre/get_server_notice",  Post);
    ADD_METHOD_TO(GameLoginCtrl::refresh_token,    "/user/login/pre/refresh_token",      Post);
    
    ADD_METHOD_TO(GameLoginCtrl::create_role ,     "/user/login/post/create_role",       Post);
    ADD_METHOD_TO(GameLoginCtrl::get_server_list , "/user/login/post/get_server_list",   Post);
    ADD_METHOD_TO(GameLoginCtrl::pre_create_role , "/user/login/post/pre_create_role",   Post);
    
    
    
    METHOD_LIST_END


    
};