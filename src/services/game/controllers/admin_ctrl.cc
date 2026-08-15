#include "admin_ctrl.h"

#include "plugins/redis/cmds/redis_cmd_flushdb.h"
#include "plugins/redis/cmds/hset/redis_cmd_hmget.h"
#include "plugins/redis/cmds/hset/redis_cmd_hmset.h"
#include "plugins/redis/cmds/zrank/redis_cmd_zadd.h"
#include "plugins/message/game_defines.h"
#include "plugins/message/redis_structs.h"

#include <chrono>
#include <functional>
#include <string>
#include <vector>

namespace
{
    struct TestAccountSeed
    {
        const char* account;
        const char* birthYear;
        int passLevel;
        int itemNum;
        int diamondCount;
        int unlockedHeroCount;
        bool blankRole;
    };

    const std::vector<TestAccountSeed> kTestAccounts = {
        {"1001", "1988", 1, 0, 0, 0, true},
        {"1002", "1988", 1, 0, 0, 0, true},
        {"1003", "1988", 1, 0, 0, 0, true},
        {"2001", "1988", 6, 10, 0, 0, false},
        {"2002", "1988", 6, 10, 0, 0, false},
        {"2003", "1988", 6, 10, 0, 0, false},
        {"2011", "1988", 251, 100, 500, 25, false},
        {"2012", "1988", 251, 100, 500, 25, false},
        {"2013", "1988", 251, 100, 500, 25, false},
        {"2021", "1988", 501, 200, 1000, 50, false},
        {"2022", "1988", 501, 200, 1000, 50, false},
        {"2023", "1988", 501, 200, 1000, 50, false},
        {"3001", "2020", 6, 10, 0, 0, false},
        {"3002", "2020", 6, 10, 0, 0, false},
        {"3003", "2020", 6, 10, 0, 0, false},
        {"3011", "2020", 251, 100, 0, 25, false},
        {"3012", "2020", 251, 100, 0, 25, false},
        {"3013", "2020", 251, 100, 0, 25, false},
        {"3021", "2020", 501, 150, 0, 50, false},
        {"3022", "2020", 501, 150, 0, 50, false},
        {"3023", "2020", 501, 150, 0, 50, false},
        {"4001", "2012", 6, 10, 0, 0, false},
        {"4002", "2012", 6, 10, 0, 0, false},
        {"4003", "2012", 6, 10, 0, 0, false},
        {"4011", "2012", 251, 100, 200, 25, false},
        {"4012", "2012", 251, 100, 200, 25, false},
        {"4013", "2012", 251, 100, 200, 25, false},
        {"4021", "2009", 501, 200, 500, 50, false},
        {"4022", "2009", 501, 200, 500, 50, false},
        {"4023", "2009", 501, 200, 500, 50, false},
        {"5001", "2009", 6, 10, 0, 0, false},
        {"5002", "2009", 6, 10, 0, 0, false},
        {"5003", "2009", 6, 10, 0, 0, false},
        {"5011", "2009", 251, 100, 200, 25, false},
        {"5012", "2009", 251, 100, 200, 25, false},
        {"5013", "2009", 251, 100, 200, 25, false},
        {"5021", "2009", 501, 200, 500, 50, false},
        {"5022", "2009", 501, 200, 500, 50, false},
        {"5023", "2009", 501, 200, 500, 50, false},
    };

    const char* kFamilyNames[] = {
        "李", "王", "张", "刘", "陈", "杨", "黄", "赵", "周", "吴",
        "徐", "孙", "胡", "朱", "高", "林", "何", "郭", "马", "罗"
    };
    const char* kGivenNames[] = {
        "晨曦", "子轩", "语桐", "浩然", "欣怡", "宇航", "思远", "雨桐", "嘉言", "景行",
        "若琳", "明哲", "安然", "清妍", "俊杰", "可欣", "星宇", "知远", "梦瑶", "逸凡"
    };

    std::string MakeRandomNickname(const std::string& account)
    {
        auto tick = std::chrono::steady_clock::now().time_since_epoch().count();
        auto seed = std::hash<std::string>{}(account + std::to_string(tick));
        constexpr size_t familyNameCount = sizeof(kFamilyNames) / sizeof(kFamilyNames[0]);
        constexpr size_t givenNameCount = sizeof(kGivenNames) / sizeof(kGivenNames[0]);
        return std::string(kFamilyNames[seed % familyNameCount]) + kGivenNames[(seed / familyNameCount) % givenNameCount];
    }

    std::string MakeUnlockedHeroes(int count)
    {
        std::string result;
        for (int heroId = 1; heroId <= count; ++heroId)
        {
            if (!result.empty()) result += ",";
            result += std::to_string(heroId);
        }
        return result;
    }

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
            std::string nickname = seed.blankRole ? "" : MakeRandomNickname(account);
            std::string unlockedHeroes = MakeUnlockedHeroes(seed.unlockedHeroCount);

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
            userReq.AddField(RedisUserData::NickName(), nickname);
            userReq.AddField(RedisUserData::HeadIcon(), seed.blankRole ? "" : "tile_1");
            userReq.AddField(RedisUserData::BirthYear(), seed.birthYear);
            userReq.AddField(RedisUserData::TodayFirstLogin(), "0");
            userReq.AddField(RedisUserData::PassLevelKey(), std::to_string(seed.passLevel));
            userReq.AddField(RedisUserData::IsFinishTutorialKey(), seed.blankRole ? "0" : "1");
            userReq.AddField(RedisUserData::ItemNum_Undo(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_Shuffle(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_Hint(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_ExtraSlot(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::ItemNum_AddTime(), std::to_string(seed.itemNum));
            userReq.AddField(RedisUserData::DiamondCount(), std::to_string(seed.diamondCount));
            userReq.AddField(RedisUserData::MonthlyRechargeMonth(), "");
            userReq.AddField(RedisUserData::MonthlyRechargeAmount(), "0");
            userReq.AddField(RedisUserData::UnlockedHeroes(), unlockedHeroes);
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

void AdminCtrl::patch_birth_year(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto jsonObject = req->jsonObject();
        if (jsonObject == nullptr || (*jsonObject)["Key"].asString() != "admin_patch_2026")
        {
            this->ErrorResponse(GameErrorCode::Fail, "Invalid admin key", callback);
            co_return;
        }

        const std::vector<std::pair<std::string, std::string>> patches = {
            {"4021", "2009"},
            {"4022", "2009"},
            {"4023", "2009"},
        };

        Json::Value updatedAccounts(Json::arrayValue);
        for (const auto& patch : patches)
        {
            const auto& account = patch.first;
            const auto& birthYear = patch.second;

            redis::cmd::hash::HMGet accountReq(RedisAccont::MakeKey(account));
            accountReq.AddField(RedisAccont::UserIdKey());
            auto ret = co_await accountReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to load account", callback);
                co_return;
            }
            auto userId = accountReq.GetFieldValue(RedisAccont::UserIdKey());
            if (userId.empty()) userId = account;

            redis::cmd::hash::HMSet userReq(RedisUserData::MakeKey(userId));
            userReq.AddField(RedisUserData::BirthYear(), birthYear);
            ret = co_await userReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to update birth year", callback);
                co_return;
            }

            Json::Value item;
            item["Account"] = account;
            item["UserId"] = userId;
            item["BirthYear"] = birthYear;
            updatedAccounts.append(item);
        }

        Json::Value respJson;
        respJson["code"] = 0;
        respJson["accounts"] = updatedAccounts;
        auto resp = HttpResponse::newHttpJsonResponse(std::move(respJson));
        callback(resp);
    })();
}

void AdminCtrl::clear_diamond(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto jsonObject = req->jsonObject();
        if (jsonObject == nullptr || (*jsonObject)["Key"].asString() != "admin_patch_2026")
        {
            this->ErrorResponse(GameErrorCode::Fail, "Invalid admin key", callback);
            co_return;
        }

        const std::vector<std::string> accounts = {"4001", "5001", "5002"};

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
            if (userId.empty()) userId = account;

            redis::cmd::hash::HMSet userReq(RedisUserData::MakeKey(userId));
            userReq.AddField(RedisUserData::DiamondCount(), "0");
            ret = co_await userReq.coExecute();
            if (ret != redis::RedisErrno::RE_Succ)
            {
                this->ErrorResponse(GameErrorCode::DBError, "Failed to clear diamond", callback);
                co_return;
            }

            Json::Value item;
            item["Account"] = account;
            item["UserId"] = userId;
            updatedAccounts.append(item);
        }

        Json::Value respJson;
        respJson["code"] = 0;
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

void AdminCtrl::test(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    Json::Value checks(Json::arrayValue);

    auto addCheck = [&](const std::string& name, bool pass, const std::string& detail = "") {
        Json::Value item;
        item["name"] = name;
        item["pass"] = pass;
        if (!detail.empty()) item["detail"] = detail;
        checks.append(item);
    };

    addCheck("seed_account_count",
        kTestAccounts.size() == 39,
        "expect 39, got " + std::to_string(kTestAccounts.size()));

    const std::vector<std::string> requiredAccounts = {
        "3003","3013","3023","4003","4013","4023","5003","5013","5023"
    };
    bool newAccountsPresent = true;
    for (const auto& acct : requiredAccounts)
    {
        auto it = std::find_if(kTestAccounts.begin(), kTestAccounts.end(),
            [&](const TestAccountSeed& s) { return std::string(s.account) == acct; });
        if (it == kTestAccounts.end()) { newAccountsPresent = false; break; }
    }
    addCheck("new_minor_accounts_present", newAccountsPresent, "3003/3013/3023/4003/4013/4023/5003/5013/5023");

    bool midLevelCorrect = true;
    for (const auto& s : kTestAccounts)
    {
        std::string acct(s.account);
        bool isMid = (acct == "2011" || acct == "2012" || acct == "2013" ||
                      acct == "3011" || acct == "3012" || acct == "3013" ||
                      acct == "4011" || acct == "4012" || acct == "4013" ||
                      acct == "5011" || acct == "5012" || acct == "5013");
        if (isMid && s.passLevel != 251) { midLevelCorrect = false; break; }
    }
    addCheck("mid_level_pass_251", midLevelCorrect);

    bool highLevelCorrect = true;
    for (const auto& s : kTestAccounts)
    {
        std::string acct(s.account);
        bool isHigh = (acct == "2021" || acct == "2022" || acct == "2023" ||
                       acct == "3021" || acct == "3022" || acct == "3023" ||
                       acct == "4021" || acct == "4022" || acct == "4023" ||
                       acct == "5021" || acct == "5022" || acct == "5023");
        if (isHigh && s.passLevel != 501) { highLevelCorrect = false; break; }
    }
    addCheck("high_level_pass_501", highLevelCorrect);

    bool ageCorrect = true;
    for (const auto& s : kTestAccounts)
    {
        std::string acct(s.account);
        std::string by(s.birthYear);
        if (acct[0] == '3' && by != "2020") { ageCorrect = false; break; }
        if ((acct == "4001" || acct == "4002" || acct == "4003" ||
             acct == "4011" || acct == "4012" || acct == "4013") && by != "2012") { ageCorrect = false; break; }
        if ((acct == "4021" || acct == "4022" || acct == "4023") && by != "2009") { ageCorrect = false; break; }
        if (acct[0] == '5' && by != "2009") { ageCorrect = false; break; }
    }
    addCheck("birth_year_correct", ageCorrect);

    std::string nick1 = MakeRandomNickname("2011");
    std::string nick2 = MakeRandomNickname("2012");
    addCheck("random_nickname_not_empty", !nick1.empty() && !nick2.empty());
    addCheck("random_nickname_differs", nick1 != nick2, nick1 + " vs " + nick2);

    addCheck("heroes_mid_25",  MakeUnlockedHeroes(25).find("25") != std::string::npos);
    addCheck("heroes_high_50", MakeUnlockedHeroes(50).find("50") != std::string::npos);

    int passCount = 0;
    for (const auto& c : checks) { if (c["pass"].asBool()) passCount++; }

    Json::Value resp_json;
    resp_json["code"] = 0;
    resp_json["total"] = static_cast<int>(checks.size());
    resp_json["passed"] = passCount;
    resp_json["failed"] = static_cast<int>(checks.size()) - passCount;
    resp_json["checks"] = checks;

    auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
    callback(resp);
}
