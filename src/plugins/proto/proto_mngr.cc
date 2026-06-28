
#include "proto_mngr.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/message.h"


void game::ProtoRegistry::initAndStart(const Json::Value &config)
{   

    this->initProtoRegistryV2();
}

void game::ProtoRegistry::shutdown() 
{
    /// Shutdown the plugin
}


// 注册消息ID关联字典
void game::ProtoRegistry::initProtoRegistryV2()
{
    auto pool = google::protobuf::DescriptorPool::generated_pool();
    google::protobuf::DescriptorDatabase* db = pool->internal_generated_database();
    if (db == nullptr) {
        return;
    }
    
    std::vector<std::string> file_names;
    db->FindAllFileNames(&file_names);   // 遍历得到所有proto文件名
    for (const std::string& filename : file_names)
    {
        const google::protobuf::FileDescriptor* fileDescriptor = pool->FindFileByName(filename);
        if (fileDescriptor == nullptr)
        {
            continue;
        }
        int enum_cnt = fileDescriptor->enum_type_count();
        for (int i = 0; i < enum_cnt; i++)
        {
            const google::protobuf::EnumDescriptor* enum_descriptor = fileDescriptor->enum_type(i);
            
            for (int enum_index = 0;enum_index < enum_descriptor->value_count(); ++enum_index)
            {
                auto cmdDesc = enum_descriptor->value(enum_index);
                auto cmdFullName = cmdDesc->name();
                auto cmdID       = cmdDesc->number();
                auto cmdMsgName = cmdFullName.substr(4, cmdFullName.length() - 4);//C2S_ S2C_

                auto desc = pool->FindMessageTypeByName(cmdMsgName);
                
                this->RegisterCmd(cmdID, desc);
                //registry_Index
            }
        }        
    }
}

void game::ProtoRegistry::RegisterCmd(int cmdID, const google::protobuf::Descriptor* des)
{
    if (nullptr == des)
    {
        return;
    }
    
    this->registry[cmdID] = des;
    this->registry_Index[des] = cmdID;
}

// 根据ID创建消息结构还是一样
google::protobuf::Message* game::ProtoRegistry::CreateMessage(int msgId)
{
    auto iter = this->registry.find(msgId);
    if (iter == this->registry.end())
    {
        return nullptr;
    }
    auto* protoType = google::protobuf::MessageFactory::generated_factory()->GetPrototype(iter->second);
    if (protoType != nullptr)
    {
        return protoType->New();
    }
    return nullptr;
}