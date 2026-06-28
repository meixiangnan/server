/**
 *
 *  proto_mngr.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>
#include <google/protobuf/descriptor.h>

#include "extends/ExMessageMsg.pb.h"

namespace game
{

class ProtoRegistry : public drogon::Plugin<ProtoRegistry>
{
public:
    ProtoRegistry() = default;

public: //plugin framework    
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

public:
    //interface
    google::protobuf::Message* CreateMessage(int msgId);

    template<typename TMsg>
    int GetCmdIdOfMessage()
    {
        auto desc = TMsg::GetDescriptor();
        return GetCmdIdOfDesc(desc);
    }

    int GetCmdIdOfMessage(const google::protobuf::Message* msg)
    {
        return GetCmdIdOfDesc(msg->GetDescriptor());
    }

private:
    int GetCmdIdOfDesc(const google::protobuf::Descriptor* desc)
    {
        if (nullptr == desc)
        {
            return -1;
        }

        auto iter = registry_Index.find(desc);
        if (iter == registry_Index.end())
        {
            return -1;
        }
        return iter->second;
    }
    
private:
    void initProtoRegistryV2();
    void RegisterCmd(int, const google::protobuf::Descriptor*);
    

public://interface
    std::unordered_map<int, const google::protobuf::Descriptor*> registry;
    std::unordered_map<const google::protobuf::Descriptor*,int > registry_Index;
};

    
}
