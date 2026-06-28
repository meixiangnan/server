#pragma once
#include <string>
#include <cstdint>
#include "drogon/orm/Field.h"
#include "drogon/orm/Row.h"
#include "trantor/utils/Date.h"
#include "trantor/utils/MsgBuffer.h"

//#include <rfl/json.hpp>

class EquipItem
{
    
};

class AttrItem
{
    
};

class RoleItem
{
public:
    int64_t ItemPstid;
    int64_t OwnerRoleId;
    int32_t TemplateId;
    trantor::Date CreateTime;
    std::string ItemName;
    int32_t BelongDressId;
    int32_t StateFlags;
    trantor::MsgBuffer EquipList;
    trantor::MsgBuffer AttrList;

    std::vector<int32_t> equips;
    std::map<int32_t, int32_t> attrs;


    static std::string GetTableName()
    {
        return "role_item";
    }
    static std::string GetLoadSql()
    {
        return "SELECT id,owner,template_id,create_time,name,dress_id,state_flags,equip_list,attr_list FROM role_item WHERE id = ?";
    }
    static std::string GetLoadAllSql()
    {
        return "SELECT id,owner,template_id,create_time,name,dress_id,state_flags,equip_list,attr_list FROM role_item";
    }
    static std::string GetUpdateSql(const RoleItem& obj)
    {
        return "INSERT INTO role_item (id, owner, template_id, create_time, name, dress_id, state_flags, equip_list, attr_list) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?) ON DUPLICATE KEY UPDATE owner = VALUES(owner), template_id = VALUES(template_id), create_time = VALUES(create_time), name = VALUES(name), dress_id = VALUES(dress_id), state_flags = VALUES(state_flags), equip_list = VALUES(equip_list), attr_list = VALUES(attr_list)";
    }
    int UnSerializeFromDb(const drogon::orm::Row &row)
    {
        ItemPstid = row[0].as<int64_t>();
        OwnerRoleId = row[1].as<int64_t>();
        TemplateId = row[2].as<int32_t>();
        CreateTime = row[3].as<trantor::Date>();
        ItemName = row[4].as<std::string>();
        BelongDressId = row[5].as<int32_t>();
        StateFlags = row[6].as<int32_t>();
        EquipList = row[7].as<trantor::MsgBuffer>();
        AttrList = row[8].as<trantor::MsgBuffer>();
        return 0;
    }

};


