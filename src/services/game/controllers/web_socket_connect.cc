#include "web_socket_connect.h"

#include <google/protobuf/descriptor.h>

#include "plugins/common_info/base_lib.h"
#include "plugins/player_mngr/game_player.h"
#include "plugins/player_mngr/player.h"
#include "plugins/player_mngr/player_mngr.h"
#include "plugins/proto/proto_mngr.h"
#include "plugins/proto/extends/ExMessageMsg.pb.h"


namespace mngr
{
    class redis_mngr;
}

void WebSocketConnnector::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &conn)
{
    auto player_mngr = app().getPlugin<mngr::PlayerMngr>();
    auto newPlayer = player_mngr->CreatePlayer();
    
    newPlayer->BindConnection(conn);
    conn->setContext(newPlayer);

    [this, newPlayer]() -> AsyncTask {
        co_await newPlayer->OnLoginLoad();
    }();
   
}



void WebSocketConnnector::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    //Record Player Leave
}

void WebSocketConnnector::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
     [this](const WebSocketConnectionPtr &wsConnPtr, std::string &&message, const WebSocketMessageType &type) -> AsyncTask {
         auto &player = wsConnPtr->getContextRef<GamePlayer>();
         player.Online();
         player.buffer.append(message);
        
         auto ProtoPlug = app().getPlugin<game::ProtoRegistry>();
        
         int readErr = 0;
         do
         {
             game::CPack recvPack;        
             readErr = recvPack.FromNetMsg(player.buffer);
             if(0 == readErr)
             {
                 if(!player.RecvSeq(recvPack.seqId))
                 {                
                     break;
                 }
                 auto newMsg = ProtoPlug->CreateMessage(recvPack.cmd);
                 if(newMsg == nullptr)
                 {
                     player.buffer.retrieveAll();
                     //unknown msg ,kick player
                     break;
                 }

                 if(!newMsg->ParseFromString(recvPack.body))
                 {
                     player.buffer.retrieveAll();
                     break;
                 }
                 co_await player.HandleMsg(recvPack.cmd, newMsg);
             }
         }
         while (readErr == 0);

         trantor::MsgBuffer leftBuffer;
         leftBuffer.append(player.buffer);
         player.buffer.swap(leftBuffer);
         co_return;
        }(wsConnPtr, std::move(message), std::move(type));    
}

int WebSocketConnnector::TryUnpackFromBuffer(game::CPack& ret, trantor::MsgBuffer& buffer)
{
    return ret.FromNetMsg(buffer);    
}


