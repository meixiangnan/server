#include "web_socket_connect.h"

//#include "third_party/drogon/lib/src/ConfigAdapterManager.h"


void WebSocketConnnector::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &conn)
{
   
}



void WebSocketConnnector::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    //Record Player Leave
}

void WebSocketConnnector::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    try
    {
        //auto configJsonRoot_ = ConfigAdapterManager::instance().getJson(message, "json");

        //auto code = configJsonRoot_["code"].asInt();
        //auto data = configJsonRoot_["data"];
        
    }
    catch (std::exception &e)
    {
        std::cout<<"Error reading json " << e.what() << '\n';
    }
}


