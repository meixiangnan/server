#include "drogon/HttpController.h"

using namespace drogon;

class MiniGameLoginCtrl : public HttpController<MiniGameLoginCtrl>
{
    void quick_login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void create(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void create_role(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    
    void upload_score(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void load_leaderboard(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
private:
    void ErrorResponse(int code, const std::string& msg,std::function<void(const HttpResponsePtr&)> callback);
    int  AdultLoginCheck(int age, int& leftSecond);
    bool IsFirstLoginToday(const std::string& lastLogin, const trantor::Date& curTime);
    bool IsLegalHoliday(const tm& date);
    int  GetAge(const std::string& strBirthYear);

public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(MiniGameLoginCtrl::quick_login,    "/user/quick_login", Post);
    ADD_METHOD_TO(MiniGameLoginCtrl::login,          "/user/login", Post);
    ADD_METHOD_TO(MiniGameLoginCtrl::create,         "/user/create", Post);
    ADD_METHOD_TO(MiniGameLoginCtrl::create_role,    "/user/create_role", Post);
    
    ADD_METHOD_TO(MiniGameLoginCtrl::upload_score,    "/game/upload_pass_level",        Post);    
    ADD_METHOD_TO(MiniGameLoginCtrl::load_leaderboard,"/game/load_leaderboard",         Post);
    METHOD_LIST_END
};