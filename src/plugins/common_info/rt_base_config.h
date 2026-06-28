/**
 *
 *  game_config.h
 *
 */

#pragma once


#include "trantor/utils/MsgBuffer.h"
#include <json/value.h>

namespace game
{
    class RtBaseConfig
    {

    private:
        static const std::string configPath;
        
    public:
        int account_cnt = 1000;
        int add_robot_cnt_per_tick = 100;
        int min_online_sec = 1000;
        int max_online_sec = 1000;
        int sleep_cnt      =  0;
        int module_interval_sec = 5;
        int channel_id = 101;
        int channel_uid_from = 40192003;
        int app_id = 1001;
        int enter_server_id = 3;
        int print_online_cnt_interval = 10;
        
        std::string role_name_prefix;        
        std::string web_auth_url;
        std::string web_get_srv_list_url;
        std::string web_create_role_url;
        std::string web_refresh_token_url;


    public:
        void Init();
        

    private:
        void ParseFrom(const Json::Value &config)
        {
            this->account_cnt            = config["account_cnt"].asInt();
            this->add_robot_cnt_per_tick = config["add_robot_cnt_per_tick"].asInt();
            this->min_online_sec         = config["min_online_sec"].asInt();
            this->max_online_sec         = config["max_online_sec"].asInt();
            this->sleep_cnt              = config["sleep_cnt"].asInt();

            this->module_interval_sec = config["module_interval_sec"].asInt();
            this->channel_id          = config["channel_id"].asInt();
            this->channel_uid_from    = config["channel_uid_from"].asInt();
            this->app_id              = config["app_id"].asInt();
            this->enter_server_id     = config["enter_server_id"].asInt();
            
            this->print_online_cnt_interval = config["print_online_cnt_interval"].asInt();

            role_name_prefix     = config["role_name_prefix"].asString();        
            web_auth_url         = config["web_auth_url"].asString();
            web_get_srv_list_url = config["web_get_srv_list_url"].asString();
            web_create_role_url  = config["web_create_role_url"].asString();
            web_refresh_token_url= config["web_refresh_token_url"].asString();
        }
    };

    
    
}
