#pragma once

enum ENUM_ITEM_MAINTYPE
{
    // 3位数的 item maintype
    IMT_PLAYER_COIN             = 100, // 玩家代币。 不支持条件判断，仅消耗
    IMT_PLAYER_ATTR             = 101, // 玩家属性：军衔，在线时长，创角时间，充值，VIP等级，经验值，上次上线时间，上次下线时间 。支持条件判断，
    IMT_PLAYER_SLOT             = 102, // 全局插槽。光环，天气，阵营，头像，看板角色。 玩家设置游戏设置捏脸，外观设置单独项

    IMT_CARD_ATTR               = 111, // 卡属性：等级，经验，星级，品质。支持条件判断，
    IMT_CARD_EQUIP_SLOT         = 112, // 卡装备插槽 
    IMT_CARD_SKILL_SLOT         = 113, // 卡技能插槽
    IMT_CARD_OUTLOOK_SLOT       = 114, // 卡外观插槽
    IMT_CARD_EQUIP_SLOT_LOCK    = 115, // 卡装备插槽锁定状态

    IMT_WEAPON_ATTR             = 121, // 武器属性：等级，经验，星级，品质。支持条件判断，
    IMT_ATTACHMENT_SLOT         = 122, // 武器配件插槽
    IMT_WEAPON_RANDOM_PROP_SLOT = 123, // 武器随机属性插槽。副属性或词条
    IMT_WEAPON_CARD_SLOT        = 124, // 装备到哪张卡上
    IMT_WEAPON_SKIN_SLOT        = 125, // 武器外观插槽

    IMT_ATTACHMENT_SLOT_LOCK    = 127, // 武器装配插槽锁定状态

    IMT_BATTLE_ATTR             = 131, // 战斗属性

    IMT_TECH_SLOT               = 132, // 科技养成插槽
    //- 4位数的 item maintype
    IMT_ITEM_CARD               = 200, // 角色卡
    IMT_ITEM_SERVANT            = 201, // 随从卡

    //- 5位数的 item maintype
    IMT_ITEM_EQUIP              = 300, // 30001武器，30002头，30003甲

    //- 6位数的 item maintype
    IMT_ITEM_SKIN               = 301, // 角色皮肤，武器皮肤等

    IMT_ITEM_ROLE_ACCESSORY     = 302, //角色配件
    //- 7位数的 item maintype
    IMT_ITEM_MAT                = 400, // 普通堆叠材料
    IMT_ITEM_TECH               = 460, // 科技树类物品，载具，支援科技
    IMT_ITEM_TECH_SKILL         = 461, // 科技树类物品, 技能
    IMT_ITEM_VDIAMOND           = 471, // 虚拟宝石
    IMT_ITEM_FITTING            = 472, // 武器配件
    IMT_ITEM_HELMET_DIAMOND     = 473, // 头盔宝石
    IMT_ITEM_ARMORT_DIAMOND     = 474, // 护甲宝石
    IMT_ITEM_SKILL_DIAMOND      = 475, // 技能宝石

    IMT_ITEM_TASK               = 481, // 任务
    IMT_ITEM_TASKCOUNT          = 482, // 任务进度
    IMT_ITEM_LOGIC              = 483, // 逻辑值

    IMT_ITEM_TEAM               = 487, // 编队
    //- 3位数的 item maintype
    IMT_ITEM_SKILL              = 500, // 技能

    IMT_ITEM_BATTLEFIELD        = 501, // 
};

// 道具具体类型。ItemType /100 = ItemMainType
//-@class BoomItemType
enum ENUM_ITEM_TYPE
{
    //- 3位数的 item type
    IT_PLAYER_COIN                          = 10000, // 玩家代币。 不支持条件判断，仅消耗
    IT_PLAYER_EXP_ATTR                      = 10111, // 玩家属性-经验：科技树经验值等
    IT_PLAYER_ATTR                          = 10113, // 玩家属性-等级: 军衔，VIP，battlepass   支持条件判断
    IT_PLAYER_STATUS                        = 10114, //玩家状态 上次上线时间，上次下线时间 创角时间，在线时长，
    IT_PLAYER_GAMESET                       = 10220,  // 玩家设置
            
    IT_CARD_ATTR                            = 11130, // 卡属性：等级，经验，星级，品质。支持条件判断，
    IT_CARD_EQUIP_SLOT                      = 11231, // 卡装备插槽
    IT_CARD_SKILL_SLOT                      = 11332, // 卡技能插槽
    IT_CARD_OUTLOOK_SLOT                    = 11433, // 卡外观插槽
            
    IT_WEAPON_ATTR                          = 12140, // 武器属性：等级，经验，星级，品质。支持条件判断，
    IT_ATTACHMENT_SLOT                      = 12241, // 武器配件插槽
    IT_WEAPON_RANDOM_PROP_SLOT              = 12342, // 武器随机属性插槽。副属性或词条
    IT_WEAPON_CARD_SLOT                     = 12440, // 装备到哪张卡上
    IT_WEAPON_SKIN_SLOT                     = 12540, // 武器用哪个皮肤
    IT_BATTLE_ATTR                          = 13100, // 战斗属性
            
    // 4位数的 item type            
    IT_ITEM_CARD                            = 20000, // 角色卡
    IT_ITEM_SARVANT                         = 20100, // 随从卡
                
    // 5位数的 item type            
    IT_ITEM_EQUIP_WEAPON                    = 30001, // 武器
    IT_ITEM_EQUIP_HELMET                    = 30002, // 头盔
    IT_ITEM_EQUIP_ARMORT                    = 30003, // 护甲
            
    // 6位数的 item type            
    IT_ITEM_SKIN                            = 30100, // 角色皮肤
    IT_ITEM_FUNCTION_SKIN                   = 30101, // 角色皮肤-功能类
      
    // 角色槽位配件      
    IT_ITEM_ACCESSORY_1                     = 30201, // 角色槽位1配件
    IT_ITEM_ACCESSORY_2                     = 30202, // 角色槽位2配件
    IT_ITEM_ACCESSORY_3                     = 30203, // 角色槽位3配件
    IT_ITEM_ACCESSORY_4                     = 30204, // 角色槽位4配件
    IT_ITEM_ACCESSORY_5                     = 30205, // 角色槽位5配件
    IT_ITEM_ACCESSORY_6                     = 30206, // 角色槽位6配件
    // 7位数的 item type            
    IT_ITEM_EQUIP_FRAGMENT                  = 40001, // 装备碎片
    IT_ITEM_SERVANT_FRAGMENT                = 40012, // 随从碎片
    IT_ITEM_PROP                            = 40020, // 使用道具
    IT_ITEM_BOX                             = 40040, // 箱子
    IT_ITEM_EQUIP_ATTR                      = 40041, // 武器属性
    IT_ITEM_EQUIP_ENHANCE_MATERIAL          = 40050, // 武器升级材料
    IT_ITEM_ROLE_ENHANCE_MATERIAL           = 40051, // 角色升级材料
    IT_ITEM_ROLE_ACCESSORY_ENHANCE_MATERIAL = 40052, //角色套装配件升级材料
    
    IT_ITEM_SPECISL_DROP                    = 40060, // 局内特殊掉落物品  
    IT_ITEM_TECH_VECHICLE                   = 46001,  //科技树类物品-载具
    IT_ITEM_TECH_SUPPORT                    = 46002,  //科技树类物品-支援道具
            
    IT_ITEM_VDIAMOND                        = 47100, // 虚拟宝石
            
    IT_ITEM_FITTING_MAGAZINE                = 47210, //  武器配件-弹夹
    IT_ITEM_FITTING_MUZZLE                  = 47220, //  武器配件-枪口
    IT_ITEM_FITTING_UPPERRAIL               = 47230, //  武器配件-倍镜
    IT_ITEM_FITTING_LOWERRAIL               = 47240, //  武器配件-握把
    IT_ITEM_FITTING_STOCK                   = 47250, //  武器配件-枪托
            
    IT_ITEM_HELMET_DIAMOND                  = 47300, // 头盔宝石
    IT_ITEM_ARMORT_DIAMOND                  = 47400, // 护甲宝石
            
    IT_ITEM_SKILL_DIAMOND1                  = 47510, // 技能宝石1
    IT_ITEM_SKILL_DIAMOND2                  = 47520, // 技能宝石2
    IT_ITEM_SKILL_DIAMOND3                  = 47530, // 技能宝石3
    IT_ITEM_SKILL_DIAMOND4                  = 47540, // 技能宝石4
            
            
    IT_ITEM_TASK                            = 48100, // 普通任务
    IT_ITEM_TASKCOUNT                       = 48200, // 普通任务进度
    //IT_ITEM_LOGIC                         = 47801, // 逻辑值
    IT_ITEM_LOGIC_ACTIVITY_DAYS             = 48305, // 逻辑值-活动进行天数
    IT_ITEM_LOGIC_CAMPAIGN_STAR_NUM         = 48310, //逻辑值-关卡通关总星数
            
    IT_ITEM_TEAM                            = 48700, // 普通编队
    // 8位数的 item type            
    IT_ITEM_SUPPORT_SKILL                   = 50001, // 支援技能
    IT_ITEM_CHARACTOR_SKILL                 = 50002, // 角色技能
    IT_ITEM_VEHICLE_SKILL                   = 50003, // 车辆技能
};


// 道具具体类型。ItemType /100 = ItemMainType
//-@class BoomCoinDefine
enum ENUM_COIN 
{
    GOLD        = 101, // 金币
    DIAMOND_PAY = 102, // 付费钻石
    DIAMOND     = 103, // 免费钻石
    ACTIONPOINT = 107, // 体力值
};

//-@class BoomPropertyDefine
enum ENUM_PLAYER_PROPERTIES 
{
    ROLE_TECH_EXP     = 104, //角色科技树经验
    VEHICLE_TECH_EXP  = 105, //载具科技树经验
    SKILL_TECH_EXP    = 106, //支援技科技树经验
    MILITARY_RANK_EXP = 110, //军衔经验
    VIP_EXP           = 118, //VIP经验
    BATTLEPASS_EXP    = 119, //battlepass经验
    MILITARY_RANK     = 130, //军衔
    VIP_LEVEL         = 131, //VIP等级
    BATTLEPASS_LEVEL  = 132, //battlepass等级
    CREATEROLE_TIME   = 140,  //创角时间
    LAST_LOGIN        = 141,  //上次上线时间
    LAST_LOGOUT       = 142,  //上次下线时间
    TOTAL_PLAYED      = 143,  //总计在线时长
};

enum ENUM_CARD_PROPERTIES 
{
    CARD_LEVEL_EXP            = 300, // 升级经验
    CARD_LEVEL                = 301, // 等级
    CARD_STAR                 = 302, // 星级
    CARD_BREAKTHROUGH_LV      = 303, // 突破阶级
    EQUIPED_WEAPON            = 311, // 武器插槽
    EQUIPED_HELMET            = 312, // 头盔插槽
    EQUIPED_ARMOR             = 313, // 护甲插槽
    EQUIPED_WEAPON_SECONDARY  = 314, // 副武器插槽
 
    EQUIPED_ABILITY1          = 321, // 技能插槽1
    EQUIPED_ABILITY2          = 322, // 技能插槽2
    EQUIPED_ABILITY3          = 323, // 技能插槽3
    EQUIPED_ABILITY4          = 324, // 技能插槽4
      
    CURRENT_CARD_SKIN         = 330, // 皮肤外观插槽
      
    SLOT_ACCESSORY_1          = 341, // 配件插槽1
    SLOT_ACCESSORY_2          = 342, // 配件插槽2
    SLOT_ACCESSORY_3          = 343, // 配件插槽3
    SLOT_ACCESSORY_4          = 344, // 配件插槽4
    SLOT_ACCESSORY_5          = 345, // 配件插槽5
    SLOT_ACCESSORY_6          = 346, // 配件插槽6
};

enum ENUM_EQUIP_PROPERTIES 
{
    EQUIP_LEVEL_EXP         = 400, // 升级经验
    EQUIP_LEVEL             = 401, // 等级
    //EQUIP_STAR_EXP          = 402, // 星级经验
    EQUIP_BREAKTHROUGH_LV   = 402, // 突破阶级
    EQUIP_STAR              = 403, // 星级    //only
    EQUIPED_BY_ROLE         = 404, // 绑定角色
    CURRENT_SKIN            = 405, // 皮肤
    EQUIP_COUNT             = 406, // 普通状态数量
     
    EQUIPED_FITTING1        = 411, // 配件插槽1
    EQUIPED_FITTING2        = 412, // 配件插槽2
    EQUIPED_FITTING3        = 413, // 配件插槽3
    EQUIPED_FITTING4        = 414, // 配件插槽4
    EQUIPED_FITTING5        = 415, // 配件插槽5
     
    EQUIPED_DIAMOND1        = 421, // 宝石插槽1
    EQUIPED_DIAMOND2        = 422, // 宝石插槽2
    EQUIPED_DIAMOND3        = 423, // 宝石插槽3
    EQUIPED_DIAMOND4        = 424, // 宝石插槽4
    EQUIPED_DIAMOND5        = 425, // 宝石插槽5
    EQUIPED_DIAMOND6        = 426, // 宝石插槽6
    EQUIPED_DIAMOND7        = 427, // 宝石插槽7
    EQUIPED_DIAMOND8        = 428, // 宝石插槽8
    EQUIPED_DIAMOND9        = 429, // 宝石插槽9
    EQUIPED_DIAMOND10       = 430, // 宝石插槽10

    EQUIPED_FITTING1_LOCK   = 501, // 配件插槽1锁定状态
    EQUIPED_FITTING2_LOCK   = 502, // 配件插槽2锁定状态
    EQUIPED_FITTING3_LOCK   = 503, // 配件插槽3锁定状态
    EQUIPED_FITTING4_LOCK   = 504, // 配件插槽4锁定状态
    EQUIPED_FITTING5_LOCK   = 505, // 配件插槽5锁定状态
};

enum ENUM_SERVANT_PROPERTIES
{
    SERVANT_EXP  = 300,  //随从卡经验
    SERVANT_LV   = 301,  //随从卡等级
    SERVANT_STAR = 302,  //随从卡星级
};

enum ENUM_BATTLE_PROPERTIES 
{
    ATTR_START_INDEX               = 431, // 逻辑判断
    ATTR_HP                        = 431, // 生命上限
    ATTR_DAMAGE                    = 432, // 伤害	
    ATTR_DEF                       = 433, // 防御
    ATTR_CRIT                      = 434, // 爆头率
    ATTR_CRITRED                   = 435, // 爆头率减少
    ATTR_ARMOR                     = 436, // 护甲
    ATTR_ARMORPEN                  = 437, // 穿甲
    ATTR_DAMAGE_REDUCERATE         = 438, // 伤害衰减率	
    ATTR_DAMAGE_REDUCERANGE        = 439, // 伤害衰减距离	
    ATTR_BULLET_SPEED              = 440, // 子弹速度	
    ATTR_BULLET_RANGE              = 441, // 子弹射程	
    ATTR_CLIP_SIZE                 = 442, // 子弹射程	
    ATTR_RELOAD_SPEED              = 443, // 重装填时间
    ATTR_ROUND_PER_SECOND          = 444, // 射速 
    ATTR_BULLET_PER_SHOOT          = 445, // 单发子弹数量
    ATTR_MIN_RECOIL                = 446, // 最小后坐力
    ATTR_MAX_RECOIL                = 447, // 最大后坐力
    ATTR_RECOIL_UP                 = 448, // 开火后坐力增加率
    ATTR_RECOIL_DOWN               = 449, // 停火后坐力恢复率
    ATTR_SIGHT_DISTANCE            = 450, // 视野
    ATTR_SIGHT_HEIGHT              = 451, // 瞄准视野高度  
    ATTR_SIGHT_LOCK_SPEED          = 452, // 自瞄锁定速度
    ATTR_SIGHT_LOCK_RANGE          = 453, // 自瞄锁定范围
    ATTR_SIGHT_SHOT_DELAY          = 454, // 自瞄射击延迟（仅狙击枪）
    ATTR_MOVE_SPEED                = 455, // 移动速度
    ATTR_ROTATE_SPEED              = 458, // 转向速度	
    ATTR_AIM_MOVE_SPEED_RATE       = 461, // 瞄准时移动速度修正
    ATTR_AIM_ROTATE_SPEED_RATE     = 463, // 瞄准时的转向速度修正
    ATTR_CRIT_DAMAGE               = 465, // 爆头伤害
    ATTR_CRITDMGRED                = 466, // 暴击伤害减免
    ATTR_WEAPONDMG                 = 467, // 武器伤害
    ATTR_THROUGH                   = 468, // 穿透等级
    ATTR_ANTITHROUGH               = 469, // 反穿透等级
    ATTR_HIT                       = 470, // 命中率
    ATTR_AVOID                     = 471, // 跳弹率
    ATTR_DMGTYPE                   = 472, // 特殊攻击类型
    ATTR_DEFTYPE                   = 473, // 特殊防御类型
    ATTR_DMGADD                    = 474, // 伤害增加
    ATTR_DMGRED					   = 475, // 伤害减免
    ATTR_KILLPOWER				   = 476, // 能量、怒气

    ATTR_END_INDEX                 = 477, // 逻辑判断
};

enum ENUM_ADVANCEBATTLE_PROPERTIES 
{
    ATTR_FIREPOWER = 620,           //威力
    ATTR_BULLETRANGE = 621,         //有效射程
    ATTR_SHOOTACCURACY = 622,       //稳定性
    ATTR_FIRERATE = 623,            // 射击效率
    ATTR_CLIPSIZE = 624,            //子弹数
    ATTR_SURVIVALABILITY = 625,     //生存
    ATTR_ATTACK = 626,              //攻击
    ATTR_SPEED = 627,               //移速
    ATTR_RANGE = 628,               //射程
};

enum ENUM_SKILL_PROPERTIES
{
    SKILL_START                 = 601,
    SKILL_BASEDAMAGE            = 601,      // 基础攻击力
    SKILL_RANGE                 = 602,      // 半径范围/厘米
    SKILL_COUNT                 = 603,      // 数量
    SKILL_SUBCOUNT              = 604,      // 技能生产物数量
    SKILL_DURATION              = 605,      // 持续时间不同技能不同解读
    SKILL_INTERVAL              = 606,      // 时间间隔
    SKILL_HEALTH                = 607,      // 血量
    SkILL_CD                    = 608,      // CD
    SKILL_ITEMID                = 609,
    SKILL_DAMAGERATE            = 610,
    SKILL_END                   = 611,
};

enum class ENUM_FITTING_PROPERTIES
{
    NORMAL       = 510,   //普通状态
    EQUIP        = 511,   //已装备
    LOCK         = 512,   //已锁定
    EQUIPANDLOCK = 513,   //已装备且已锁定
};

enum ENUM_TECH_PROPERTIES 
{
    TECH_LEVEL   = 514,   //科技物品等级
};

enum ENUM_TOWER_SLOT
{
    PASSED_LEVEL      = 701,   //通关层数
    REWARD_LEVEL      = 702,   //可领奖层数
    DAILY_REWARD_FLAG = 703,   //每日领奖标记
};

enum ENUM_PVPOBJ_PROERTIES 
{
    // 目前预留1-9是待选任务槽位
    OptionalMisson_1        = 1,
    OptionalMisson_2        = 2,
    OptionalMisson_3        = 3,

    EBFSlot_Stage          = 10,  // 战前准备阶段槽位
    EBFSlot_ChosenMission  = 11,  // 已选择的任务
    EBFSlot_ConfirmedRP    = 12,  // 确定带入局内的RP值
    EBFSlot_ConfirmedTeamID= 13,  // 已确认的随从编队id
};

enum BagType
{
    Currency         = 0,   //货币仓库
    Role             = 1,   //角色仓库
    Equip            = 2,   //装备仓库
	Dress            = 3,   //时装仓库
	Fitting          = 4,   //实例宝石仓库
    Material         = 5,   //材料背包
	Tech             = 6,   //科技物品仓库
	Team             = 7,   //编队列表
	BattleField      = 8,   //战场列表
	Servant          = 9,   //随从列表
    Accessory        = 10,  //角色套装
    TimeLimitItem    = 15,  //限时物品列表
    ProgressionProps = 16,  //养成类道具(材料仓库)
    ConsumableProps  = 17,  //消耗类道具(道具仓库)
    Tower            = 18,  //爬塔数据背包
    Max              = 19,
};

//奖励物品额外标记
enum RewardItemExFlag
{
    NONE              = 0,  //无
    NEW_GET           = 1,  //新获得
    MVP_GET           = 2,  //MVP获取
    GAME_DROP         = 3,  //局内掉落拾取
    GAME_FIRSTPASS    = 4,  //局内首通奖励
    GAME_RANK         = 5,  //局内排名奖励
    GAME_KILL         = 6,  //局内击杀奖励
};

//物品属性显示类型
enum class ItemAttrShowType
{
    NORMAL    = 1, //显示为普通值
    PERCENT   = 2, //显示为百分比值
};

enum WeaponSubType 
{
    Assault_Rifle             = 1, //突击步枪
    Shot_Gun                  = 2, //散弹枪
    Sniper_Rifle              = 3, //狙击步枪
    Light_MachineGun          = 4, //轻机枪
    Miniature_Submachine_Gun  = 5, //微型冲锋枪
};