
#include "game_version_ctrl.h"

#include "plugins/game_config.h"

void GameVerionCtrl::GetVersion(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto conf= app().getPlugin<game::config>();
    auto RtConf = conf->RtConf();
        
    Json::Value resp_json;
    resp_json["res_uri"]  = RtConf.web_create_role_url;
    resp_json["web_addr"] = RtConf.web_get_srv_list_url;
    resp_json["web_addr_bak1"] = RtConf.web_get_srv_list_url;
    resp_json["web_addr_bak2"] = RtConf.web_get_srv_list_url;
    resp_json["app_id"]   = RtConf.app_id;
    resp_json["is_review"]   = false;
    resp_json["app_ver"]   = 1;
    resp_json["res_ver"]   = 1;
    

    callback(HttpResponse::newHttpJsonResponse(std::move(resp_json)));
}

void GameVerionCtrl::GetVsnId(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto conf= app().getPlugin<game::config>();
    auto RtConf = conf->RtConf();

    auto& reqJson = *(req->jsonObject().get());
    
    auto channel_id  = reqJson["channel_id"].asInt();
    auto app_id      = reqJson["app_id"].asInt();
    auto ver         = reqJson["ver"].asString();        

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody("1.0");

    callback(resp);
}

/**
* {
    "app_id": 1001,
    "channel_id": 101,
    "device_id": "fdd062d7d60bfd9ce3ec39d57e31e88e34f52d24",
    "device_mod": "B250M-Wind (Gigabyte Technology Co._ Ltd.)",
    "opsys": "Windows 10  (10.0.0) 64bit",
    "graphics_device": "NVIDIA GeForce GTX 1070",
    "processor_tp": "Intel(R) Core(TM) i7-7700 CPU @ 3.60GHz",
    "processor_fre": 3600,
    "graphics_mem": 8088,
    "system_mem": 32726,
    "resolution": "1920 x 1080 @ 60Hz",
    "shader_lv": 50
}
 */
void GameVerionCtrl::PhoneInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    //parse PhoneInfo and register to db

    
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    callback(resp);
}



void GameVerionCtrl::LogFlag(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{

    //req  { "app_id": 1001, "channel_id": 101, "device_id": "fdd062d7d60bfd9ce3ec39d57e31e88e34f52d24", "flag": 10, "ostype": "WindowsEditor", "version": "0.1.0", "network": "wifi" }
    
    
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    callback(resp);
}
