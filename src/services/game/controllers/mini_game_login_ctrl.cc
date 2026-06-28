
#include "mini_game_login_ctrl.h"

#include "plugins/game_config.h"
#include "plugins/common_info/id_card_validator.h"

static std::string TrimJsonQuotes(const std::string& s)
{
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
    {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

static std::string GetDisplayNickName(const std::string& raw)
{
    std::string name = TrimJsonQuotes(raw);
    if (!name.empty())
    {
        return name;
    }

    static const char* adjectives[] = {
        "快乐的", "勇敢的", "聪明的", "神秘的", "闪亮的",
        "可爱的", "威武的", "活泼的", "灵动的", "潇洒的"
    };
    static const char* nouns[] = {
        "魔法师", "冒险者", "骑士", "精灵", "战士",
        "法师", "勇者", "猎人", "游侠", "守卫"
    };

    srand((unsigned int)std::hash<std::string>{}(raw + std::to_string(rand())));
    int ai = rand() % 10;
    int ni = rand() % 10;
    return std::string(adjectives[ai]) + std::string(nouns[ni]);
}
#include "plugins/game_modules/leaderboard_mngr/leaderboard_mngr.h"
#include "plugins/message/game_defines.h"
#include "plugins/message/redis_structs.h"
#include "plugins/redis/cmds/hset/redis_cmd_hgetall.h"
#include "plugins/redis/cmds/hset/redis_cmd_hmget.h"
#include "plugins/redis/cmds/hset/redis_cmd_hmset.h"
#include "plugins/redis/cmds/string/redis_cmd_exists.h"
#include "plugins/redis/cmds/string/redis_cmd_get.h"
#include "plugins/redis/cmds/string/redis_cmd_incr.h"
#include "plugins/redis/cmds/string/redis_cmd_setnx.h"
#include "plugins/redis/cmds/zrank/redis_cmd_zadd.h"
#include "plugins/redis/cmds/zrank/redis_cmd_zrange.h"
#include <plugins/time_mngr/time_mngr.h>

namespace
{
    std::string OrZero(const std::string& value)
    {
        return value.empty() ? "0" : value;
    }
}


void MiniGameLoginCtrl::quick_login(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]()  -> Task<void>
    {
        auto& reqJson = *(req->jsonObject().get());
        LOG_INFO << "req:" << req->body() << "\n";
        auto account = reqJson[RedisAccont::AccountKey()].asString();
        auto userId  = reqJson[RedisAccont::UserIdKey()].asString();
        auto token   = reqJson[RedisAccont::TokenKey()].asString();

        redis::cmd::string::Get cmd(RedisLoginTokenGen::MakeKey(account, userId));        
        auto ret = co_await cmd.coExecute();

        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::InvaildAccount, "InvaildAccount", callback);
            co_return;
        }

        if (token != cmd.getValue())
        {
            this->ErrorResponse(GameErrorCode::InvaildToken, "InvaildToken", callback);
            co_return;
        }

        redis::cmd::hash::HMGet LoadUserData(RedisUserData::MakeKey(userId));        
        LoadUserData.AddField(RedisUserData::NickName());
        LoadUserData.AddField(RedisUserData::HeadIcon());
        LoadUserData.AddField(RedisUserData::BirthYear());
        LoadUserData.AddField(RedisUserData::TodayFirstLogin());
        LoadUserData.AddField(RedisUserData::PassLevelKey());
        LoadUserData.AddField(RedisUserData::IsFinishTutorialKey());
        LoadUserData.AddField(RedisUserData::ItemNum_Undo());
        LoadUserData.AddField(RedisUserData::ItemNum_Shuffle());
        LoadUserData.AddField(RedisUserData::ItemNum_Hint());
        LoadUserData.AddField(RedisUserData::ItemNum_ExtraSlot());
        LoadUserData.AddField(RedisUserData::ItemNum_AddTime());
        LoadUserData.AddField(RedisUserData::DiamondCount());
        LoadUserData.AddField(RedisUserData::MonthlyRechargeMonth());
        LoadUserData.AddField(RedisUserData::MonthlyRechargeAmount());
        LoadUserData.AddField(RedisUserData::UnlockedHeroes());

        ret = co_await LoadUserData.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
           this->ErrorResponse(GameErrorCode::InvaildUserId, "InvaildUserId", callback);
           co_return;
        }

        int leftSeconds = 0;
		auto age = GetAge(LoadUserData.GetFieldValue(RedisUserData::BirthYear()));
		GameErrorCode error = (GameErrorCode)AdultLoginCheck(age, leftSeconds);
		if (error != GameErrorCode::Succ)
		{
            Json::Value err_json;
            err_json["code"] = error;
            err_json["msg"]  = "AdultLoginCheck Error";
            err_json["Age"]  = age;
            auto err_resp = HttpResponse::newHttpJsonResponse(std::move(err_json));
            callback(err_resp);
			co_return;
		}
        
        TokenGenerator::Gen(token);
        int tokenTTl = 30 * 24 * 60 * 60;
        redis::cmd::string::SetEX genToken(RedisLoginTokenGen::MakeKey(account, userId), tokenTTl, token);        
        co_await genToken.coExecute();        
        
        Json::Value data_json;

        data_json[RedisAccont  ::UserIdKey()] = userId;
        data_json[RedisAccont  ::TokenKey()] = token;
        
        data_json[RedisUserData::HeadIcon()]  = LoadUserData.GetFieldValue(RedisUserData::HeadIcon());
        data_json[RedisUserData::NickName() ] = LoadUserData.GetFieldValue(RedisUserData::NickName());
        data_json[RedisUserData::BirthYear() ] = LoadUserData.GetFieldValue(RedisUserData::BirthYear());
        
        data_json[RedisUserData::PassLevelKey()] = LoadUserData.GetFieldValue(RedisUserData::PassLevelKey());
        data_json[RedisUserData::IsFinishTutorialKey()]= LoadUserData.GetFieldValue(RedisUserData::IsFinishTutorialKey());
        
        data_json[RedisUserData::ItemNum_Undo() ]    = LoadUserData.GetFieldValue(RedisUserData::ItemNum_Undo());
        data_json[RedisUserData::ItemNum_Shuffle()]  = LoadUserData.GetFieldValue(RedisUserData::ItemNum_Shuffle());
        data_json[RedisUserData::ItemNum_Hint() ]    = LoadUserData.GetFieldValue(RedisUserData::ItemNum_Hint());
        data_json[RedisUserData::ItemNum_ExtraSlot()]= LoadUserData.GetFieldValue(RedisUserData::ItemNum_ExtraSlot());
        data_json[RedisUserData::ItemNum_AddTime()]  = LoadUserData.GetFieldValue(RedisUserData::ItemNum_AddTime());
        data_json[RedisUserData::DiamondCount()] = OrZero(LoadUserData.GetFieldValue(RedisUserData::DiamondCount()));
        data_json[RedisUserData::MonthlyRechargeMonth()] = LoadUserData.GetFieldValue(RedisUserData::MonthlyRechargeMonth());
        data_json[RedisUserData::MonthlyRechargeAmount()] = OrZero(LoadUserData.GetFieldValue(RedisUserData::MonthlyRechargeAmount()));
        data_json[RedisUserData::UnlockedHeroes()] = LoadUserData.GetFieldValue(RedisUserData::UnlockedHeroes());

        data_json[ConstStr::LeftSeconds()] = leftSeconds;

        Json::Value resp_json;
        resp_json["code"]= 0;
        resp_json["data"]  = data_json;

        Json::StreamWriterBuilder jsonBuilder;
        LOG_INFO << "req:" << req->body() <<  "\n" << "resp:" << writeString(jsonBuilder, resp_json);
        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
        callback(resp);  
        
        
    })();
}

void MiniGameLoginCtrl::login(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]()  -> Task<void>
    {        
        auto& reqJson = *(req->jsonObject().get());
        LOG_INFO << "req:" << req->body() << "\n";
        auto account_id      = reqJson["Account" ].asString();
        auto pwdMd5          = reqJson["Password"].asString();

        auto serveTime = app().getPlugin<mngr::time_mngr>()->GetServerDate();
        auto str = serveTime.toDbString();

        redis::cmd::hash::HMGet cmd(RedisAccont::MakeKey(account_id));
        cmd.AddField(RedisAccont::PasswordKey());
        cmd.AddField(RedisAccont::UserIdKey());
        auto ret = co_await cmd.coExecute();
        
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::InvaildAccount, "InvaildAccount", callback);
            co_return;
        }

        auto curPwd = cmd.GetFieldValue(RedisAccont::PasswordKey());
        if ( curPwd != pwdMd5)
        {
            this->ErrorResponse(GameErrorCode::PasswordError, "PasswordError", callback);
            co_return;
        }
        auto curUserId = cmd.GetFieldValue(RedisAccont::UserIdKey());

        if (curUserId.empty())
        {
            this->ErrorResponse(GameErrorCode::InvaildAccount, "InvaildAccount", callback);
            co_return;
        }

        redis::cmd::hash::HMGet LoadUserData(RedisUserData::MakeKey(curUserId));
        LoadUserData.AddField(RedisUserData::HeadIcon());
        LoadUserData.AddField(RedisUserData::NickName());
        LoadUserData.AddField(RedisUserData::BirthYear());
        LoadUserData.AddField(RedisUserData::TodayFirstLogin());
        LoadUserData.AddField(RedisUserData::PassLevelKey());
        LoadUserData.AddField(RedisUserData::IsFinishTutorialKey());
        LoadUserData.AddField(RedisUserData::ItemNum_Undo());
        LoadUserData.AddField(RedisUserData::ItemNum_Shuffle());
        LoadUserData.AddField(RedisUserData::ItemNum_Hint());
        LoadUserData.AddField(RedisUserData::ItemNum_ExtraSlot());
        LoadUserData.AddField(RedisUserData::ItemNum_AddTime());
        LoadUserData.AddField(RedisUserData::DiamondCount());
        LoadUserData.AddField(RedisUserData::MonthlyRechargeMonth());
        LoadUserData.AddField(RedisUserData::MonthlyRechargeAmount());
        LoadUserData.AddField(RedisUserData::UnlockedHeroes());

        ret = co_await LoadUserData.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::InvaildUserId, "InvaildUserId", callback);
            co_return;
        }
        
        int leftSeconds = 0;
        auto age = GetAge(LoadUserData.GetFieldValue(RedisUserData::BirthYear()));
        GameErrorCode error = (GameErrorCode)AdultLoginCheck(age, leftSeconds);
		if (error != GameErrorCode::Succ)
        {
            Json::Value err_json;
            err_json["code"] = error;
            err_json["msg"]  = "AdultLoginCheck Error";
            err_json["Age"]  = age;
            auto err_resp = HttpResponse::newHttpJsonResponse(std::move(err_json));
            callback(err_resp);
			co_return;
        }

        std::string token;
        TokenGenerator::Gen(token);
        int tokenTTl = 30 * 24 * 60 * 60;
        redis::cmd::string::SetEX genToken(RedisLoginTokenGen::MakeKey(account_id, curUserId), tokenTTl, token);        
        co_await genToken.coExecute();

        Json::Value data_json;
        data_json[RedisUserData::HeadIcon()]  = LoadUserData.GetFieldValue(RedisUserData::HeadIcon());
        data_json[RedisUserData::NickName() ] = LoadUserData.GetFieldValue(RedisUserData::NickName());
        data_json[RedisUserData::BirthYear() ] = LoadUserData.GetFieldValue(RedisUserData::BirthYear());
        data_json[RedisUserData::PassLevelKey()] = LoadUserData.GetFieldValue(RedisUserData::PassLevelKey());
        data_json[RedisUserData::IsFinishTutorialKey()]= LoadUserData.GetFieldValue(RedisUserData::IsFinishTutorialKey());

        data_json[RedisUserData::ItemNum_Undo() ]    = LoadUserData.GetFieldValue(RedisUserData::ItemNum_Undo());
        data_json[RedisUserData::ItemNum_Shuffle()]  = LoadUserData.GetFieldValue(RedisUserData::ItemNum_Shuffle());
        data_json[RedisUserData::ItemNum_Hint() ]    = LoadUserData.GetFieldValue(RedisUserData::ItemNum_Hint());
        data_json[RedisUserData::ItemNum_ExtraSlot()]= LoadUserData.GetFieldValue(RedisUserData::ItemNum_ExtraSlot());
        data_json[RedisUserData::ItemNum_AddTime()]  = LoadUserData.GetFieldValue(RedisUserData::ItemNum_AddTime());
        data_json[RedisUserData::DiamondCount()] = OrZero(LoadUserData.GetFieldValue(RedisUserData::DiamondCount()));
        data_json[RedisUserData::MonthlyRechargeMonth()] = LoadUserData.GetFieldValue(RedisUserData::MonthlyRechargeMonth());
        data_json[RedisUserData::MonthlyRechargeAmount()] = OrZero(LoadUserData.GetFieldValue(RedisUserData::MonthlyRechargeAmount()));
        data_json[RedisUserData::UnlockedHeroes()] = LoadUserData.GetFieldValue(RedisUserData::UnlockedHeroes());
        data_json[ConstStr::LeftSeconds()] = leftSeconds;
        
        data_json[RedisAccont  ::UserIdKey()] = curUserId;
        data_json[RedisAccont  ::AccountKey()] = account_id;
        data_json[RedisAccont  ::TokenKey()] = token;
    
        Json::Value resp_json;
        resp_json["code"]= 0;
        resp_json["data"]  = data_json;

        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
        Json::StreamWriterBuilder jsonBuilder;
        LOG_INFO << "req:" << req->body() << "\n" << "resp:" << writeString(jsonBuilder, resp_json);
        callback(resp);  
    })();
}

void MiniGameLoginCtrl::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto& reqJson = *(req->jsonObject().get());
        LOG_INFO << "req:" << req->body() << "\n";
        auto account_id = reqJson["Account" ].asString();
        auto pwdMd5     = reqJson["Password"].asString();
        
        auto IdName     = reqJson["IdName"].asString();
        auto IdCard     = reqJson["IdCard"].asString();

        if (account_id.empty() || IdName.empty() || IdCard.empty() || pwdMd5.empty())
        {
            this->ErrorResponse(GameErrorCode::Fail, "paramError", callback);
            co_return;
        }

        redis::cmd::string::Exists checkExsit(RedisAccont::MakeKey(account_id));
        auto checkRet = co_await checkExsit.coExecute();
        if (checkRet != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::Fail, "Fail", callback);
            co_return;
        }

        if (checkExsit.GetExists())
        {
            this->ErrorResponse(GameErrorCode::AccountExists, "AccountExists", callback);
            co_return;
        }
        
        
        if (!IdCardVailidater::ValidatorLength(IdCard))
        {
            this->ErrorResponse(GameErrorCode::IdCardLengthError, "", callback);
            co_return;
        }
        
        if (!IdCardVailidater::ValidatorDigit(IdCard))
        {
            this->ErrorResponse(GameErrorCode::IdCardDigitError, "", callback);
            co_return;
        }
        
        int year = std::stoi(IdCard.substr(6, 4));
        int age = trantor::Date::now().tmStruct().tm_year  + 1900 - year;

        if (!IdCardVailidater::ValidatorAge(age))
        {
            this->ErrorResponse(GameErrorCode::IdCardAgeError, "", callback);
            co_return;
        }

        if (!IdCardVailidater::ValidatorAreaCode(IdCard))
        {
            this->ErrorResponse(GameErrorCode::IdCardAreaError, "", callback);
            co_return;
        }
        
        if (!IdCardVailidater::ValidatorCheckSum(IdCard))
        {
            this->ErrorResponse(GameErrorCode::IdCardCheckSumError, "", callback);
            co_return;
        }
        
        redis::cmd::string::Incr idGen(RedisUserIdGen::Key());
        auto ret = co_await idGen.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
            co_return;
        }

        auto userId = std::to_string(idGen.getValue());

        std::string token;
        TokenGenerator::Gen(token);
        int tokenTTl = 30 * 24 * 60 * 60;
        redis::cmd::string::SetEX genToken(RedisLoginTokenGen::MakeKey(account_id, userId), tokenTTl, token);        
        co_await genToken.coExecute();
        
        redis::cmd::hash::HMSet cmd(RedisAccont::MakeKey(account_id));
        cmd.AddField(RedisAccont::PasswordKey(), pwdMd5);
        cmd.AddField(RedisAccont::UserIdKey(), userId);
        
        ret = co_await cmd.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
            co_return;
        }
        
        auto curTime = app().getPlugin<mngr::time_mngr>()->GetServerDate();
        auto todayFirstLogin = std::to_string(curTime.secondsSinceEpoch());
        redis::cmd::hash::HMSet userReq(RedisUserData::MakeKey(userId));                
        userReq.AddField(RedisUserData::NickName(), "");
        userReq.AddField(RedisUserData::HeadIcon(), "");
        userReq.AddField(RedisUserData::BirthYear(), std::to_string(year));

        userReq.AddField(RedisUserData::TodayFirstLogin(), todayFirstLogin);
        
        userReq.AddField(RedisUserData::PassLevelKey(), "1");
        userReq.AddField(RedisUserData::IsFinishTutorialKey(), "0");
        userReq.AddField(RedisUserData::ItemNum_Undo(), "3");
        userReq.AddField(RedisUserData::ItemNum_Shuffle(), "3");
        userReq.AddField(RedisUserData::ItemNum_Hint(), "3");
        userReq.AddField(RedisUserData::ItemNum_ExtraSlot(), "3");
        userReq.AddField(RedisUserData::ItemNum_AddTime(), "3");
        userReq.AddField(RedisUserData::DiamondCount(), "0");
        userReq.AddField(RedisUserData::MonthlyRechargeMonth(), "");
        userReq.AddField(RedisUserData::MonthlyRechargeAmount(), "0");
        userReq.AddField(RedisUserData::UnlockedHeroes(), "");

        ret = co_await userReq.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
            co_return;
        }

        Json::Value data_json;
        data_json[RedisAccont  ::UserIdKey()] = userId;
        data_json[RedisAccont  ::TokenKey()] = token;
        
        data_json[RedisUserData::BirthYear()] = std::to_string(year);        
        data_json[RedisUserData::PassLevelKey()] = 1;
        data_json[RedisUserData::IsFinishTutorialKey()]= "0";
        data_json[RedisUserData::ItemNum_Undo()]    = "3";
        data_json[RedisUserData::ItemNum_Shuffle()]  = "3";
        data_json[RedisUserData::ItemNum_Hint()]    = "3";
        data_json[RedisUserData::ItemNum_ExtraSlot()]= "3";
        data_json[RedisUserData::ItemNum_AddTime()]= "3";
        data_json[RedisUserData::DiamondCount()] = "0";
        data_json[RedisUserData::MonthlyRechargeMonth()] = "";
        data_json[RedisUserData::MonthlyRechargeAmount()] = "0";
        data_json[RedisUserData::UnlockedHeroes()] = "";
    
        Json::Value resp_json;
        resp_json["code"]= 0;
        resp_json["data"]  = data_json;

        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
		Json::StreamWriterBuilder jsonBuilder;
		LOG_INFO << "req:" << req->body() << "\n" << "resp:" << writeString(jsonBuilder, resp_json);
        callback(resp);
        
    })();
}


void MiniGameLoginCtrl::create_role(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto& reqJson = *(req->jsonObject().get());
        LOG_INFO << "req:" << req->body() << "\n";
        auto userId   = reqJson[RedisCreateRole::UserId()].asString();
        auto nickName = reqJson[RedisCreateRole::NickName()].asString();
        auto headIcon = reqJson[RedisCreateRole::HeadIcon()].asString();

        if (nickName.empty())
        {
            this->ErrorResponse(GameErrorCode::NickNameIsNull, "", callback);
            co_return;
        }

        if (headIcon.empty())
        {
            this->ErrorResponse(GameErrorCode::HeadIconIsNull, "", callback);
            co_return;
        }
        
        redis::cmd::hash::HGetAll queryOld(RedisUserData::MakeKey(userId));
        auto ret = co_await queryOld.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
            co_return;
        }

        auto oldNickName = queryOld.GetFieldValue(RedisUserData::NickName());
        auto oldHeadIcon = queryOld.GetFieldValue(RedisUserData::HeadIcon());
        
        redis::cmd::hash::HMSet cmd(RedisUserData::MakeKey(userId));
        cmd.AddField(RedisUserData::NickName(), nickName);
        cmd.AddField(RedisUserData::HeadIcon(), headIcon);
        ret = co_await cmd.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
            co_return;
        }

        auto rankMngr = app().getPlugin<mngr::leaderboard_mngr>();
        rankMngr->RemoveCacheUserNickName(userId);        
        
        Json::Value data_json;
        data_json[RedisCreateRole::NickName()] = nickName;
        data_json[RedisCreateRole::HeadIcon()] = headIcon;
        
        Json::Value resp_json;
        resp_json["code"]= 0;
        resp_json["data"]  = data_json;

		Json::StreamWriterBuilder jsonBuilder;
		LOG_INFO << "req:" << req->body() << "\n" << "resp:" << writeString(jsonBuilder, resp_json);

        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));		
        callback(resp);
        
    })();
}


void MiniGameLoginCtrl::upload_score(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
   async_func([this, req, callback]() -> Task<void>
   {
       auto& reqJson = *(req->jsonObject().get());
       LOG_INFO << "req:" << req->body() << "\n";
       auto userId    = reqJson[RedisAccont::UserIdKey()].asString();       
       auto PassLevel = reqJson[RedisUserData::PassLevelKey()].asInt();
       auto IsFinishTutorial = reqJson[RedisUserData::IsFinishTutorialKey()].asInt();
       auto ItemNum_Undo = reqJson[RedisUserData::ItemNum_Undo()].asInt();
       auto ItemNum_Shuffle = reqJson[RedisUserData::ItemNum_Shuffle()].asInt();
       auto ItemNum_Hint = reqJson[RedisUserData::ItemNum_Hint()].asInt();
       auto ItemNum_ExtraSlot = reqJson[RedisUserData::ItemNum_ExtraSlot()].asInt();
       auto ItemNum_AddTime = reqJson[RedisUserData::ItemNum_AddTime()].asInt();
       auto DiamondCount = reqJson[RedisUserData::DiamondCount()].asInt();
       auto MonthlyRechargeMonth = reqJson[RedisUserData::MonthlyRechargeMonth()].asString();
       auto MonthlyRechargeAmount = reqJson[RedisUserData::MonthlyRechargeAmount()].asInt();
       auto UnlockedHeroes = reqJson[RedisUserData::UnlockedHeroes()].asString();

       redis::cmd::hash::HMSet updateUser(RedisUserData::MakeKey(userId));
       updateUser.AddField(RedisUserData::PassLevelKey(), std::to_string(PassLevel));
       updateUser.AddField(RedisUserData::IsFinishTutorialKey(), std::to_string(IsFinishTutorial));
       updateUser.AddField(RedisUserData::ItemNum_Undo(), std::to_string(ItemNum_Undo));
       updateUser.AddField(RedisUserData::ItemNum_Shuffle(), std::to_string(ItemNum_Shuffle));
       updateUser.AddField(RedisUserData::ItemNum_Hint(), std::to_string(ItemNum_Hint));
       updateUser.AddField(RedisUserData::ItemNum_ExtraSlot(), std::to_string(ItemNum_ExtraSlot));
       updateUser.AddField(RedisUserData::ItemNum_AddTime(), std::to_string(ItemNum_AddTime));
       updateUser.AddField(RedisUserData::DiamondCount(), std::to_string(DiamondCount));
       updateUser.AddField(RedisUserData::MonthlyRechargeMonth(), MonthlyRechargeMonth);
       updateUser.AddField(RedisUserData::MonthlyRechargeAmount(), std::to_string(MonthlyRechargeAmount));
       updateUser.AddField(RedisUserData::UnlockedHeroes(), UnlockedHeroes);
       
       co_await updateUser.coExecute();
       
       redis::cmd::hash::ZAdd userReq(RedisPassLevelRank::MakeKey());
       userReq.AddScore(userId, PassLevel);
       auto ret = co_await userReq.coExecute();
       if (ret != redis::RedisErrno::RE_Succ)
       {
           this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
           co_return;
       }
       
       Json::Value resp_json;
       resp_json["code"]= 0;
	   Json::StreamWriterBuilder jsonBuilder;
	   LOG_INFO << "req:" << req->body() << "\n" << "resp:" << writeString(jsonBuilder, resp_json);

       auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
       callback(resp);
   })();
}

void MiniGameLoginCtrl::load_leaderboard(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    async_func([this, req, callback]() -> Task<void>
    {
        auto& reqJson = *(req->jsonObject().get());
        LOG_INFO << "req:" << req->body() << "\n";
        auto userId    = reqJson[RedisAccont::UserIdKey()].asString();
        
        redis::cmd::hash::ZRevRange userReq(RedisPassLevelRank::MakeKey(), 0, 100);
        auto ret = co_await userReq.coExecute();
        if (ret != redis::RedisErrno::RE_Succ)
        {
            this->ErrorResponse(GameErrorCode::DBError, "DBError", callback);
            co_return;
        }

        auto rankMngr = app().getPlugin<mngr::leaderboard_mngr>();
        Json::Value rank_json(Json::arrayValue);

        std::vector<std::string> UserIds;
        for(auto& it : userReq.getResult())
        {
            UserIds.push_back(it.first);
        }
        co_await rankMngr->RefreshCache(UserIds);        

        int Rank = 1;
        for(auto& it : userReq.getResult())
        {
            auto info = rankMngr->GetUserNickName(it.first);
            if (nullptr == info)
            {
                continue;
            }
            
            Json::Value rank_item;
            rank_item["Name"] = GetDisplayNickName(info->NickName);
            rank_item["HeadIcon"] = info->HeadIcon;
            rank_item["Score"] = it.second;
            rank_item["Rank"] = Rank++;
            rank_json.append(rank_item);
        }

        redis::cmd::hash::HMGet updateUser(RedisUserData::MakeKey(userId));
        updateUser.AddField(RedisUserData::PassLevelKey());
        updateUser.AddField(RedisUserData::HeadIcon());
        updateUser.AddField(RedisUserData::NickName());
        co_await updateUser.coExecute();

        int myPass = atoi(updateUser.GetFieldValue(RedisUserData::PassLevelKey()).c_str()); 
        redis::cmd::hash::ZRevRank myRank(RedisPassLevelRank::MakeKey(), userId);
        co_await myRank.coExecute();

        Json::Value my_rank_item;
        my_rank_item["Name"] = GetDisplayNickName(updateUser.GetFieldValue(RedisUserData::NickName()));
        my_rank_item["HeadIcon"] = updateUser.GetFieldValue(RedisUserData::HeadIcon());
        my_rank_item["Score"] = updateUser.GetFieldValue(RedisUserData::PassLevelKey());
        my_rank_item["Rank"] = myRank.getRank() + 1;
        
        
        Json::Value data_json;
        data_json["RankData"]   = rank_json;
        data_json["SelfRank"] = my_rank_item;
        Json::Value resp_json;
        resp_json["code"]= 0;
        resp_json["data"]  = data_json;
		Json::StreamWriterBuilder jsonBuilder;
		LOG_INFO << "req:" << req->body() << "\n" << "resp:" << writeString(jsonBuilder, resp_json);
        auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
        callback(resp);
    })();
}

void MiniGameLoginCtrl::ErrorResponse(int code, const std::string& msg, std::function<void(const HttpResponsePtr&)> callback)
{
    Json::Value resp_json;
    resp_json["code"]= code;
    resp_json["msg"] = msg;

	Json::StreamWriterBuilder jsonBuilder;
	LOG_INFO << "ErrorResponse:" << code << " " << msg;

    auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
    callback(resp);  
}

int MiniGameLoginCtrl::AdultLoginCheck(int age, int& leftSecond)
{
    if (age >= 18) 
    {
        return GameErrorCode::Succ;
    }
    auto serveTime = app().getPlugin<mngr::time_mngr>()->GetServerDate();
    auto tmDate = serveTime.tmStruct();
    bool IsOKWeekday = (tmDate.tm_wday == 0 || tmDate.tm_wday == 5 || tmDate.tm_wday == 6);
    bool IsOKHoliday = IsLegalHoliday(tmDate);
    bool IsOKHour = (tmDate.tm_hour == 20);

    if ((!IsOKWeekday && !IsOKHoliday) || !IsOKHour) 
    {
        return GameErrorCode::AgeCannotLoginNow;
    }
    
    leftSecond =  60 * (59 - tmDate.tm_min) + (60 - tmDate.tm_sec);

    return GameErrorCode::Succ;
}

bool MiniGameLoginCtrl::IsFirstLoginToday(const std::string& lastLogin, const trantor::Date& curTime)
{
    if (lastLogin.length() <= 0) 
    {
        return true;
    }

	auto tmDate = curTime.tmStruct();
    auto lastLoginDate = trantor::Date(std::stoi(lastLogin) * MICRO_SECONDS_PRE_SEC);

    auto tmOld = lastLoginDate.tmStruct();
    if (tmOld.tm_year == tmDate.tm_year
        && tmOld.tm_mon == tmDate.tm_mon
        && tmOld.tm_mday == tmDate.tm_mday
        && lastLoginDate < curTime)
    {
        return false;
    }

    return true;
}

bool MiniGameLoginCtrl::IsLegalHoliday(const tm& date)
{
    static std::map<int, std::map<int, std::set<int>>> holidays;
    if (holidays.size() == 0) 
	{
        holidays[2025][1].insert(1);        
        holidays[2025][1].insert(28);
        holidays[2025][1].insert(29);
        holidays[2025][1].insert(30);
        holidays[2025][1].insert(31);
        holidays[2025][2].insert(1);
        holidays[2025][2].insert(2);
        holidays[2025][2].insert(3);
        holidays[2025][2].insert(4);
        holidays[2025][4].insert(4);
        holidays[2025][4].insert(5);
        holidays[2025][4].insert(6);
        holidays[2025][5].insert(1);
        holidays[2025][5].insert(2);
		holidays[2025][5].insert(3);
		holidays[2025][5].insert(4);
        holidays[2025][5].insert(5);
        holidays[2025][5].insert(31);
        holidays[2025][6].insert(1);
        holidays[2025][6].insert(2);        
        holidays[2025][10].insert(1);
        holidays[2025][10].insert(2);
        holidays[2025][10].insert(3);
		holidays[2025][10].insert(4);
		holidays[2025][10].insert(5);
		holidays[2025][10].insert(6);
		holidays[2025][10].insert(7);
        holidays[2025][10].insert(8);

		holidays[2026][1].insert(1);
        holidays[2026][1].insert(2);
        holidays[2026][1].insert(3);
		holidays[2026][2].insert(15);
		holidays[2026][2].insert(16);
		holidays[2026][2].insert(17);
		holidays[2026][2].insert(18);
		holidays[2026][2].insert(19);
		holidays[2026][2].insert(20);
		holidays[2026][2].insert(21);
        holidays[2026][2].insert(22);
        holidays[2026][2].insert(23);
		holidays[2026][4].insert(4);
		holidays[2026][4].insert(5);
		holidays[2026][4].insert(6);
		holidays[2026][5].insert(1);
		holidays[2026][5].insert(2);
		holidays[2026][5].insert(3);
		holidays[2026][5].insert(4);
		holidays[2026][5].insert(5);
		holidays[2026][6].insert(19);
		holidays[2026][6].insert(20);
		holidays[2026][6].insert(21);
		holidays[2026][9].insert(25);
        holidays[2026][9].insert(26);
		holidays[2026][9].insert(27);
		holidays[2026][10].insert(1);
		holidays[2026][10].insert(2);
		holidays[2026][10].insert(3);
		holidays[2026][10].insert(4);
		holidays[2026][10].insert(5);
		holidays[2026][10].insert(6);
		holidays[2026][10].insert(7);
    }

    int year = 1900 + date.tm_year;
    auto Month = holidays[year][date.tm_mon + 1];
    
    if (Month.find((date.tm_mday)) == Month.end())
    {
        return false;
    }
    return true;
}

int MiniGameLoginCtrl::GetAge(const std::string& strBirthYear)
{
    if (strBirthYear.length() == 0)
    {
        return 18;
    }

    int year = std::stoi(strBirthYear.c_str());
    return trantor::Date::now().tmStruct().tm_year + 1900 - year;
}
