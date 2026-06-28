
#include "game_login_ctrl.h"

#include "plugins/game_config.h"


void GameLoginCtrl::get_auth(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto& reqJson = *(req->jsonObject().get());
    
    auto channel_id      = reqJson["channel_id"     ].asInt();
    auto app_id          = reqJson["app_id"         ].asInt();
    auto channel_uid     = reqJson["channel_uid"    ].asString();
    auto device_id       = reqJson["device_id"      ].asString();
    auto access_token    = reqJson["access_token"   ].asString();
    auto session_id      = reqJson["session_id"     ].asString();
    auto ori_channel_id  = reqJson["ori_channel_id" ].asString();
    auto ori_channel_uid = reqJson["ori_channel_uid"].asString();
    auto extension       = reqJson["extension"      ].asString();

    Json::Value data_json;

    data_json["refresh_token"] = "new_token";
    data_json["expire_time"  ] = 3600;
    data_json["account_id"   ] = 5637268478;
    data_json["server_token" ] = "new_server_token";
    
    Json::Value resp_json;
    resp_json["code"]= 0;
    resp_json["data"]  = data_json;

    auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
    callback(resp);  
    
}

void GameLoginCtrl::get_server_notice(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto& reqJson = *(req->jsonObject().get());
    
    auto channel_id  = reqJson["channel_id"].asInt();
    auto app_id      = reqJson["app_id"    ].asInt();
    auto device_id   = reqJson["device_id" ].asString();


    //todo: get real notice here

    Json::Value resp_json;
    resp_json["code"]   = 0;
    resp_json["notice"] = "notice_key ?";
    resp_json["access"] = true;
    
    auto resp = HttpResponse::newHttpJsonResponse(std::move(resp_json));
    callback(resp);    
}

void GameLoginCtrl::refresh_token(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto& reqJson = *(req->jsonObject().get());
    
    auto account_id    = reqJson["account_id"   ].asInt64();
    auto refresh_token = reqJson["refresh_token"].asString();


    Json::Value data_json;

    data_json["refresh_token"] = "new_refresh_token";
    data_json["expire_time"  ] = 3600;
    data_json["server_token"]  = "server_token";
    
    Json::Value resp_json;
    resp_json["code"]= 0;
    resp_json["data"]  = data_json;

    callback(HttpResponse::newHttpJsonResponse(std::move(resp_json)));  
}

void GameLoginCtrl::create_role(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto& reqJson = *(req->jsonObject().get());
    
    auto account_id      = reqJson["account_id"     ].asInt64();
    auto server_token    = reqJson["server_token"   ].asString();
    auto role_name       = reqJson["role_name"      ].asString();
    auto server_id       = reqJson["server_id"      ].asInt();

    //app().getDbClient()
    /*
    auto sdk_channel_id  = reqJson["sdk_channel_id" ].asString();
    auto device_platform = reqJson["device_platform"].asString();
    auto client_version  = reqJson["client_version" ].asString();
    auto app_id          = reqJson["app_id"         ].asString();
    auto idfa            = reqJson["idfa"           ].asString();
    auto idfv            = reqJson["idfv"           ].asString();
    auto android_id      = reqJson["android_id"     ].asString();
    auto gaid            = reqJson["gaid"           ].asString();
    auto device_id       = reqJson["device_id"      ].asString();d
    auto adjust_id       = reqJson["adjust_id"      ].asString();
    auto device_model    = reqJson["device_model"   ].asString();
    auto cpu_info        = reqJson["cpu_info"       ].asString();
    auto device_memory   = reqJson["device_memory"  ].asString();
    */


    Json::Value data_json;

    data_json["role_name"] = role_name;
    data_json["role_id"  ] = 77777777;
    data_json["server_id"] = server_id;
    
    Json::Value resp_json;
    resp_json["code"]= 0;
    resp_json["data"]  = data_json;

    callback(HttpResponse::newHttpJsonResponse(std::move(resp_json)));  

}

void GameLoginCtrl::get_server_list(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto& reqJson = *(req->jsonObject().get());    
    auto account_id      = reqJson["account_id"     ].asInt64();
    auto server_token    = reqJson["server_token"   ].asString();

    Json::Value one_server_json;
    one_server_json["region_name" ] = "region_name";
    one_server_json["server_state"] = 77777777;
    one_server_json["switch_role" ] = 1;
    one_server_json["server_name" ] = 1;
    one_server_json["server_id"   ] = 1;

    Json::Value gate_json;
    gate_json.append("http://127.0.0.1:33018");
    one_server_json["web_gate_list"] = gate_json;

    one_server_json["recommand"] = 1;
    one_server_json["msg_gate"]  = "ws://xxx_msg_gate";
    one_server_json["game_gate"] = "ws://127.0.0.1:33055/game";
    one_server_json["region_id"] = 1;    

    Json::Value data_array_json;
    
    data_array_json.append(one_server_json);
    
    Json::Value resp_json;
    resp_json["code"]= 0;
    resp_json["data"]  = data_array_json;
    callback(HttpResponse::newHttpJsonResponse(std::move(resp_json)));  
    
}

void GameLoginCtrl::pre_create_role(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto& reqJson = *(req->jsonObject().get());    
    auto account_id   = reqJson["account_id"  ].asInt64();
    auto server_token = reqJson["server_token"].asString();
    auto server_id    = reqJson["server_id"   ].asInt();

    Json::Value resp_json;
    resp_json["code"]= 0;
    callback(HttpResponse::newHttpJsonResponse(std::move(resp_json)));  
}


