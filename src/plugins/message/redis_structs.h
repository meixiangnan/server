
#include "plugins/redis/cmds/redis_cmd_base.h"

/*
 * ID生成表
 * {
 *   "UserIdGen": 2222
 * } *
 */

class RedisUserIdGen
{
public :
    inline static std::string Key()
    {
        return "UserIdGen";
    }
};

/*
 * token记录
 * {
 *      "LoginToken:account:userId": token (with TTL)
 * }
 *      
 */
class RedisLoginTokenGen
{
public :
    inline static std::string MakeKey(const std::string& account_id, const std::string& userId)
    {
        return "LoginToken:" + account_id + ":" + userId;
    }
};

/*
 * 账号表, 记录账号密码token
 * {
 *      "Account:111":
 *      {
 *         "Password": "xxx"
 *         "UserId": "xxx"
 *        
 *      }
 * }
 */



class RedisAccont
{
public:
    inline static std::string MakeKey(const std::string& account_id)
    {
        return "Account:" + account_id;
    }

    inline static std::string PasswordKey()
    {
        return"Password";
    }
    inline static std::string UserIdKey()
    {
        return"UserId";
    }
    
    inline static std::string TokenKey()
    {
        return"Token";
    }
    
    inline static std::string AccountKey()
    {
        return"Account";
    }
};

/*
 * 玩家数据表, 记录账号游戏数据
 * {
 *      "UserData:111":
 *      { 
 *         "NickName": "xxx"
 *         "HeadIcon": "xxx"
 *         "PassLevel": "xxx"
 *         "IsFinishTutorial": "0/1"
 *         "ItemNum_Undo"："0"
 *         "ItemNum_Shuffle"："0"
 *         "ItemNum_Hint"："0"
 *         "ItemNum_ExtraSlot"："0"
 *         "ItemNum_AddTime"："0"
 *         "BirthYear": "xxx"
 *         "DailyFirstLogin": "0"
 *      }
 * }
 */

class RedisUserData
{
public:
    inline static std::string MakeKey(const std::string& account_id)
    {
        return "UserData:" + account_id;
    }

    inline static std::string NickName()
    {
        return"NickName";
    }

    inline static std::string HeadIcon()
    {
        return"HeadIcon";
    }
    
    inline static std::string BirthYear()
    {
        return"BirthYear";
    }

	inline static std::string TodayFirstLogin()
	{
		return"TodayFirstLogin";
	}
    
    inline static std::string PassLevelKey()
    {
        return"PassLevel";
    }

    inline static std::string IsFinishTutorialKey()
    {
        return"IsFinishTutorial";
    }

    inline static std::string ItemNum_Undo()
    {
        return"ItemNum_Undo";
    }
    inline static std::string ItemNum_Shuffle()
    {
        return"ItemNum_Shuffle";
    }
    inline static std::string ItemNum_Hint()
    {
        return"ItemNum_Hint";
    }
    inline static std::string ItemNum_ExtraSlot()
    {
        return"ItemNum_ExtraSlot";
    }
    inline static std::string ItemNum_AddTime()
    {
        return"ItemNum_AddTime";
    }

    inline static std::string DiamondCount()
    {
        return "DiamondCount";
    }

    inline static std::string MonthlyRechargeMonth()
    {
        return "MonthlyRechargeMonth";
    }

    inline static std::string MonthlyRechargeAmount()
    {
        return "MonthlyRechargeAmount";
    }

    inline static std::string UnlockedHeroes()
    {
        return "UnlockedHeroes";
    }
    
};

class RedisCreateUser
{
public:    
    inline static std::string NickName()
    {
        return"NickName";
    }
    inline static std::string UserId()
    {
        return"UserId";
    }    
    
};

class RedisCreateRole
{
public:
    inline static std::string UserId()
    {
        return RedisCreateUser::UserId();
    }
    
    inline static std::string NickName()
    {
        return RedisUserData::NickName();
    }

    inline static std::string HeadIcon()
    {
        return RedisUserData::HeadIcon();
    }
};


/*
 * 排行榜表, 记录通关排行榜
 * {
 *      "PassLevelRank":
 *      [
 *          [111(passLevel), 222(userId)],
 *          [110(passLevel), 222(userId)],
 *          [109(passLevel), 222(userId)],
 *      ] 
 * }
 */

class RedisPassLevelRank
{
    public:
    inline static std::string MakeKey()
    {
        return "PassLevelRank";
    }
};


class ConstStr 
{
public:
    inline static const std::string RedisDataFormat()
    {
        return "%Y-%m-%d_%H:%M:%S";
    }


	inline static const std::string LeftSeconds()
	{
		return "LeftSeconds";
	}
    
};