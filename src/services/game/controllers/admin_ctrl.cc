#include "admin_ctrl.h"

#include "plugins/redis/cmds/redis_cmd_flushdb.h"
#include "plugins/redis/cmds/hset/redis_cmd_hmget.h"
#include "plugins/redis/cmds/hset/redis_cmd_hmset.h"
#include "plugins/redis/cmds/zrank/redis_cmd_zadd.h"
#include "plugins/message/game_defines.h"
#include "plugins/message/redis_structs.h"

#include <vector>

namespace
{
    struct TestAccountSeed
    {
        const char* account;
        const char* birthYear;
        int passLevel;
        int isFinishTutorial;
        int itemNum;
        int diamondCount;
        const char* unlockedHeroes;
        bool blankRole;
    };

    const std::vector<TestAccountSeed> kTestAccounts = {
        {"1001", "1988", 1, 0, 0, 0, "", true},
        {"1002", "1988", 1, 0, 0, 0, "", true},
        {"1003", "1988", 1, 0, 0, 0, "", true},
        {"2001", "1988", 6, 1, 10, 0, "", false},
        {"2002", "1988", 6, 1, 10, 0, "", false},
        {"2003", "1988", 6, 1, 10, 0, "", false},
        {"2011", "1988", 151, 1, 100, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"2012", "1988", 151, 1, 100, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"2013", "1988", 151, 1, 100, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"2021", "1988", 301, 1, 200, 1000, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"2022", "1988", 301, 1, 200, 1000, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"2023", "1988", 301, 1, 200, 1000, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"3001", "2020", 6, 1, 10, 0, "", false},
        {"3002", "2020", 6, 1, 10, 0, "", false},
        {"3011", "2020", 151, 1, 100, 0, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"3012", "2020", 151, 1, 100, 0, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"3021", "2020", 301, 1, 150, 0, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"3022", "2020", 301, 1, 150, 0, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"4001", "2012", 6, 1, 10, 0, "", false},
        {"4002", "2012", 6, 1, 10, 0, "", false},
        {"4011", "2012", 151, 1, 100, 200, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"4012", "2012", 151, 1, 100, 200, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"4021", "2009", 301, 1, 200, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"4022", "2009", 301, 1, 200, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"5001", "2009", 6, 1, 10, 0, "", false},
        {"5002", "2009", 6, 1, 10, 0, "", false},
        {"5011", "2009", 151, 1, 100, 200, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"5012", "2009", 151, 1, 100, 200, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", false},
        {"5021", "2009", 301, 1, 200, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
        {"5022", "2009", 301, 1, 200, 500, "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30", false},
    };

    constexpr const char* kTestPasswordHash = "99a94989c5e21c6c9652753a43158950";
}

void AdminCtrl::clear_all_data(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        LOG_INFO << "Admin: clear_all_data request received";

        redis::cmd::FlushDB cmd;
        auto ret = co_await cmd.coExecute();
        
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "Failed to clear database", callback);
            co_return;
        }

        Json::Value resp_json;
        resp_json["code"] = 0;
        resp_json["message"] = "All data cleared successfully";

        LOG_INFO << "Admin: All data cleared successfully";
        
        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
        callback(resp);
    })();
}

void AdminCtrl::seed_test_accounts(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto jsonObject = req->jsonObject();
        if (jsonObject == nullptr || (*jsonObject)["Key"].asString() != "seed_test_accounts_2026")
        {
            this->ErrorResponse(GameErrorCode::Fail, "Invalid admin key", callback);
            co_return;
        }

        int updatedCount = 0;
        Json::Value updatedAccounts(Json::arrayValue);
        for (const auto& seed : kTestAccounts)
        {
            std::string account = seed.account;
            std::string userId = account;

            redis::cmd::hash::HMSet accountReq(RedisAccont::MakeKey(account));
            accountReq.AddField(RedisAccont::PasswordKey(), kTestPasswordHash);
            accountReq.AddField(RedisAccont::UserIdKey(), userId);
            auto ret = co_await accountReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to seed account", callback);
                co_return;
            }

            redis::cmd::hash::HMSet userReq(RedisUserData::MakeKey(userId));
            userReq.AddField(RedisUserData::NickName(), seed.blankRole ? "" : ("TestAccount" + account));
            userReq.AddField(RedisUserData::HeadIcon(), seed.blankRole ? "" : "tile_1");
            userReq.AddField(RedisUserData::BirthYear(), seed.birthYear);
            userReq.AddField(RedisUserData::TodayFirstLogin(), "0");
            userReq.AddField(RedisUserData::PassLevelKey(), std::to_string(seed.passLevel));
            userReq.AddField(RedisUserData::IsFinishTutorialKey(), std::to_string(seed.isFinishTutorial));
            userReq.AddField(RedisUserData::ItemNum_Undo(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_Shuffle(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_Hint(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_ExtraSlot(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_AddTime(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::DiamondCount(), std::to_string(seed.diamondCount));
            userReq.AddField(RedisUserData::MonthlyRechargeMonth(), "");
            userReq.AddField(RedisUserData::MonthlyRechargeAmount(), "0");
            userReq.AddField(RedisUserData::UnlockedHeroes(), seed.unlockedHeroes);
            ret = co_await userReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to seed user data", callback);
                co_return;
            }

            redis::cmd::hash::ZAdd rankReq(RedisPassLevelRank::MakeKey());
            rankReq.AddScore(userId, seed.passLevel);
            ret = co_await rankReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to seed rank", callback);
                co_return;
            }

            updatedCount++;
            updatedAccounts.append(account);
        }

        Json::Value resp_json;
        resp_json["code"] = 0;
        resp_json["updatedCount"] = updatedCount;
        resp_json["accounts"] = updatedAccounts;
        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
        callback(resp);
    })();
}

void AdminCtrl::fix_sixteen_minor_birth_year(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto jsonObject = req->jsonObject();
        if (jsonObject == nullptr || (*jsonObject)["Key"].asString() != "fix_sixteen_minor_birth_year_2026")
        {
            this->ErrorResponse(GameErrorCode::Fail, "Invalid admin key", callback);
            co_return;
        }

        const std::vector<std::string> accounts = {
            "4021", "4022", "5001", "5002", "5011", "5012", "5021", "5022"
        };

        Json::Value updatedAccounts(Json::arrayValue);
        for (const auto& account : accounts)
        {
            redis::cmd::hash::HMGet accountReq(RedisAccont::MakeKey(account));
            accountReq.AddField(RedisAccont::UserIdKey());
            auto ret = co_await accountReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to load account", callback);
                co_return;
            }

            auto userId = accountReq.GetFieldValue(RedisAccont::UserIdKey());
            if (userId.empty())
            {
                this->ErrorResponse(GameErrorCode::InvaildAccount, "Missing user id", callback);
                co_return;
            }

            redis::cmd::hash::HMSet userReq(RedisUserData::MakeKey(userId));
            userReq.AddField(RedisUserData::BirthYear(), "2009");
            ret = co_await userReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to update birth year", callback);
                co_return;
            }

            Json::Value item;
            item["Account"] = account;
            item["UserId"] = userId;
            item["BirthYear"] = "2009";
            updatedAccounts.append(item);
        }

        Json::Value respJson;
        respJson["code"] = 0;
        respJson["updatedCount"] = static_cast<int>(accounts.size());
        respJson["accounts"] = updatedAccounts;
        auto resp = HttpResponse::newHttpJsonResponse(std::move(respJson));
        callback(resp);
    })();
}

void AdminCtrl::ErrorResponse(int code, const std::string& msg, std::function<void(const HttpResponsePtr&)> callback)
{
    Json::Value resp_json;
    resp_json["code"] = code;
    resp_json["msg"] = msg;

    LOG_ERROR << "Admin ErrorResponse: " << code << " " << msg;

    auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
    callback(resp);
}
