#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

#include "plugins/common_info/base_lib.h"
using namespace drogon;

class WebSocketConnnector : public drogon::WebSocketController<WebSocketConnnector>
{
  public:
    void handleNewMessage(const WebSocketConnectionPtr &, std::string &&, const WebSocketMessageType &) override;
    void handleConnectionClosed(const WebSocketConnectionPtr &) override;
    void handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr &) override;
    
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/srv/ws/dsm");
    WS_PATH_LIST_END

};