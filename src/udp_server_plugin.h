#ifndef UDP_SERVER_PLUGIN_H
#define UDP_SERVER_PLUGIN_H

#include <xbot2/rt_plugin/control_plugin.h>

#include "udp_server.h"
#include "tom_centauro_udp/packet/packet_ros_utils.hpp"

#include <ec_xbot2/hand_ec.h>

using namespace XBot;

class UdpServerPlugin : public XBot::ControlPlugin
{

public:

    using ControlPlugin::ControlPlugin;

    bool on_initialize() override;
    void on_start() override;
    void run() override;
    void on_stop() override;

private:

    typedef PublisherPtr<Eigen::Affine3d> CommmandPublisherPtr;
    typedef PublisherPtr<double> GripperPublisherPtr;

    UdpServer<tom_centauro_udp::packet::master2slave,
              tom_centauro_udp::packet::slave2master> _srv;

    std::map<std::string, SubscriberPtr<Eigen::Affine3d>> _ee_subs;

    std::map<std::string, CommmandPublisherPtr> _ee_pubs;
    std::map<std::string, GripperPublisherPtr> _gripper_pubs;

    std::map<std::string, Eigen::Affine3d> _ee_state;

    SubscriberPtr<bool> _heartbeat_sub;

    int _nmsgs, _nrepl;

    chrono::steady_clock::time_point _start_time;
    chrono::steady_clock::time_point _last_heartbeat_recv_time;

    // heri hand
    std::map<std::string, std::shared_ptr<Hal::HandEc>> _heri_map;
    std::atomic<bool> _heri2_detected;
};

#endif // UDP_SERVER_PLUGIN_H
