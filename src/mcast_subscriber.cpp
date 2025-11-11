#include "mcast_socket.h"
#include "logging.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace Common;

int main() {
    Logger logger("mcast_subscriber.log");

    const std::string mcast_ip = "239.192.1.1";
    const std::string iface = "lo";
    const int port = 5001;

    McastSocket subscriber(logger);
    subscriber.init(mcast_ip, iface, port, true); // true → receiver
    subscriber.join(mcast_ip);

    subscriber.setRecvCallback([&](McastSocket* s) {
        std::string msg(s->inboundBuffer().data(), s->recvSize());
        s->resetRecvSize();
        std::cout << "📩 " << msg << std::flush;
    });

    logger.log("📡 Joined multicast group %:%\n", mcast_ip, port);

    while (true) {
        subscriber.sendAndRecv();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
