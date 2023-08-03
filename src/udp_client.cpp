#include "udp_server.h"
#include "tom_centauro_udp/packet/packet.hpp"
#include "tom_centauro_udp/packet/packet_ros_utils.hpp"

#include <Eigen/Dense>
#include <iostream>

#include <csignal>
#include <chrono>
#include <thread>
void exit_handle(int)
{
    exit(0);
}
int main(int argc, char **argv)
{
    signal(SIGINT, exit_handle);

    std::string addr = "127.0.0.1";
    int port = 8081;

    std::string ee_id = "left_ee";

    if (argc > 1)
    {
        ee_id = argv[1];
    }

    UdpClientRaw cli;
    if (!cli.init(addr, port))
    {
        exit(1);
    }

    bool got_msg_from_robot = false;

    tom_centauro_udp::packet::master2slave packet_to_robot;
    tom_centauro_udp::fill_pkt_ee_id(packet_to_robot, ee_id);
    packet_to_robot.run = false;
    packet_to_robot.vel_xy[0] = 0;
    packet_to_robot.vel_xy[1] = 0;
    packet_to_robot.vel_yaw = 0;

    tom_centauro_udp::packet::slave2master packet_from_robot;

    while (!got_msg_from_robot)
    {
        std::cout << "waiting for server to reply.." << std::endl;

        cli.try_send(reinterpret_cast<uint8_t *>(&packet_to_robot),
                     sizeof(packet_to_robot));

        packet_from_robot.magic_code = 0;
        if (cli.try_receive(reinterpret_cast<uint8_t *>(&packet_from_robot),
                            sizeof(packet_from_robot)) > 0)
        {
            if (!tom_centauro_udp::check_pkt_valid(packet_from_robot))
            {
                std::cout << "invalid packet received" << std::endl;
                continue;
            }

            if (std::string(packet_from_robot.ee_id) != ee_id)
            {
                std::cout << "packet for ee " << packet_from_robot.ee_id << "!= " << ee_id << " received" << std::endl;
                continue;
            }

            got_msg_from_robot = true;
        }

        usleep(100000);
    }

    // set run to true
    packet_to_robot.run = true;

    Eigen::Affine3d initial_pose;
    tom_centauro_udp::get_pose_from_pkt(packet_from_robot, initial_pose);
    // std::cout << "initial pose for " << packet_from_robot.ee_id << " received: \n"
    //           << initial_pose << "\n";

    double t = 0;
    const double dt = 0.001;
    const double period = 3.0;
    const double amplitude = 0.1;
    int nrepl = 0;
    auto next = std::chrono::steady_clock::now();
    for (;;)
    {
        t = next.time_since_epoch().count() * 1e-9;
        // fill pose to send
        Eigen::Affine3d pose_to_send = initial_pose;

        pose_to_send.translation().z() += amplitude * std::sin(2 * M_PI * t / period);
        pose_to_send.translation().y() += amplitude * std::cos(2 * M_PI * t / period);

        tom_centauro_udp::fill_pkt_with_pose(packet_to_robot,
                                             pose_to_send);

        // send command to slave
        cli.try_send(reinterpret_cast<uint8_t *>(&packet_to_robot),
                     sizeof(packet_to_robot));

        // try receive from robot
        packet_from_robot.magic_code = 0;
        if (cli.try_receive(reinterpret_cast<uint8_t *>(&packet_from_robot),
                            sizeof(packet_from_robot)) > 0)
        {
            if (!tom_centauro_udp::check_pkt_valid(packet_from_robot))
            {
                std::cout << "invalid packet received" << std::endl;
                continue;
            }

            if (std::string(packet_from_robot.ee_id) != ee_id)
            {
                std::cout << "packet for ee " << packet_from_robot.ee_id << "!= " << ee_id << " received" << std::endl;
                continue;
            }

            nrepl++;
        }
        next += std::chrono::milliseconds(1);
        std::this_thread::sleep_until(next);
    }
}
