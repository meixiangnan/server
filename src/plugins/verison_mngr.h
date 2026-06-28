/**
 *
 *  mngr_verison_mngr.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>
namespace mngr
{

class verison_mngr : public drogon::Plugin<verison_mngr>
{
  public:
    verison_mngr() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;
};

}
