cmake_minimum_required(VERSION 3.29)

set(COMMON_SRC 
    plugins/common_info/rt_base_config.cc
    plugins/common_info/rt_base_config.h
    plugins/common_info/base_lib.cc
    plugins/common_info/base_lib.h
    plugins/common_info/compoment.cc
    plugins/common_info/compoment.h
    plugins/common_info/id_card_validator.cc
    plugins/common_info/id_card_validator.h
)

set(PLUGIN_INTERFACE_SRC 
    plugins/game_config.h
    plugins/game_config.cc    
)

set(GAME_SVR_MAIN
    services/game/main.cc
)

set(GAME_SVR_PLUGIN_SRC
    plugins/game_config.h
    plugins/game_config.cc

    plugins/redis/redis_mngr.h
    plugins/redis/redis_mngr.cc
    plugins/redis/cmds/redis_cmd_base.h
    plugins/redis/cmds/redis_cmd_base.cc
    
    plugins/redis/cmds/hset/redis_cmd_hmget.h
    plugins/redis/cmds/hset/redis_cmd_hmget.cc
    plugins/redis/cmds/hset/redis_cmd_hmset.h
    plugins/redis/cmds/hset/redis_cmd_hmset.cc
    plugins/redis/cmds/hset/redis_cmd_hgetall.h
    plugins/redis/cmds/hset/redis_cmd_hgetall.cc
    
    plugins/redis/cmds/script/redis_cmd_evalsha.h
    plugins/redis/cmds/script/redis_cmd_evalsha.cc
    plugins/redis/cmds/script/redis_cmd_scriptload.h
    plugins/redis/cmds/script/redis_cmd_scriptload.cc    
    
    plugins/redis/cmds/string/redis_cmd_exists.h
    plugins/redis/cmds/string/redis_cmd_exists.cc
    plugins/redis/cmds/string/redis_cmd_get.h
    plugins/redis/cmds/string/redis_cmd_get.cc
    plugins/redis/cmds/string/redis_cmd_incr.h
    plugins/redis/cmds/string/redis_cmd_incr.cc
    plugins/redis/cmds/string/redis_cmd_setnx.h
    plugins/redis/cmds/string/redis_cmd_setnx.cc
    plugins/redis/cmds/string/redis_cmd_incrby.h
    plugins/redis/cmds/string/redis_cmd_incrby.cc
    
    plugins/redis/cmds/zrank/redis_cmd_zadd.h
    plugins/redis/cmds/zrank/redis_cmd_zadd.cc
    plugins/redis/cmds/zrank/redis_cmd_zrange.h
    plugins/redis/cmds/zrank/redis_cmd_zrange.cc

    plugins/task/task_mngr.h
    plugins/task/task_mngr.cc
    
    plugins/time_mngr/time_mngr.h
    plugins/time_mngr/time_mngr.cc
    
    plugins/message/game_defines.h
    plugins/message/redis_structs.h    
    
    plugins/game_modules/leaderboard_mngr/leaderboard_mngr.h
    plugins/game_modules/leaderboard_mngr/leaderboard_mngr.cc
)

set(GAME_SVR_SRC    
    services/game/controllers/mini_game_login_ctrl.h
    services/game/controllers/mini_game_login_ctrl.cc
    services/game/controllers/admin_ctrl.h
    services/game/controllers/admin_ctrl.cc
)

function (SourceGroupTargetSRC targetName)

    get_target_property(SOURCE_FILES ${targetName} SOURCES)
    foreach(FILE ${SOURCE_FILES})
        get_filename_component(REL_DIR ${FILE} DIRECTORY)
        # 替换路径分隔符为组名分隔符
        string(REPLACE "/" "\\" GROUP ${REL_DIR})        
        # 添加到对应的组
        source_group(${GROUP} FILES ${FILE})
    endforeach()
endfunction()

find_package(absl QUIET)
find_package(protobuf QUIET)

# ##############################################################################
#Game WebSocketServer


add_executable(Game ${GAME_SVR_MAIN})

target_sources(Game PRIVATE ${GAME_SVR_SRC})
target_sources(Game PRIVATE ${COMMON_SRC})
target_sources(Game PRIVATE ${GAME_SVR_PLUGIN_SRC})

set_property(TARGET Game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${publish_bin_debug}/")
target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Game PUBLIC ${BOOST_INCLUDE_DIR})

target_link_libraries(Game PUBLIC Drogon::Drogon)
#target_link_libraries(Game PRIVATE ${jsoncpp_LIBRARIES})
find_package(jsoncpp)
target_link_libraries(Game PUBLIC JsonCpp::JsonCpp)

SourceGroupTargetSRC(Game)
