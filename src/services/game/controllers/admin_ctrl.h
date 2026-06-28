#pragma once

#include "drogon/HttpController.h"

using namespace drogon;

class AdminCtrl : public HttpController<AdminCtrl>
{
    void clear_all_data(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void seed_test_accounts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void fix_sixteen_minor_birth_year(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

private:
    void ErrorResponse(int code, const std::string& msg, std::function<void(const HttpResponsePtr&)> callback);

public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(AdminCtrl::clear_all_data, "/admin/clear_all_data", Post);
    ADD_METHOD_TO(AdminCtrl::seed_test_accounts, "/admin/seed_test_accounts", Post);
    ADD_METHOD_TO(AdminCtrl::fix_sixteen_minor_birth_year, "/admin/fix_sixteen_minor_birth_year", Post);

    METHOD_LIST_END
};
