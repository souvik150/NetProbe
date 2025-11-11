#include "mcast_socket.h"
#include "logging.h"
#include <thread>
#include <random>
#include <chrono>
#include <sstream>

using namespace Common;

int main() {
    Logger logger("mcast_publisher.log");

    const std::string mcast_ip = "239.192.1.1";   // multicast group
    const std::string iface = "lo";               // loopback interface
    const int port = 5001;

    McastSocket publisher(logger);
    publisher.init(mcast_ip, iface, port, false); // false → sender

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> price(100.0, 200.0);

    logger.log("🚀 Starting Multicast Publisher on %:%\n", mcast_ip, port);

    while (true) {
        std::ostringstream oss;
        oss << "TICK|AAPL|" << price(rng) << "\n";
        std::string msg = oss.str();

        publisher.send(msg.data(), msg.size());
        publisher.sendAndRecv();

        logger.log("📤 Sent: %\n", msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
