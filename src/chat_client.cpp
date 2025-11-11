#include "tcp_socket.h"
#include "logging.h"
#include "time_utils.h"
#include <thread>
#include <iostream>

using namespace Common;

int main() {
    Logger logger("chat_client.log");

    const std::string ip = "127.0.0.1";
    const std::string iface = "lo";
    const int port = 5555;

    TCPSocket client(logger);
    client.recv_callback_ = [&](TCPSocket* socket, Nanos rx_time) noexcept {
        std::string msg(socket->inbound_data_.data(), socket->next_rcv_valid_index_);
        socket->next_rcv_valid_index_ = 0;

        // print message from server
        std::cout << "\r💬 " << msg << "\n> " << std::flush;
    };

    client.connect(ip, iface, port, false);

    // Background thread to keep receiving
    std::thread reader([&]() {
        while (true) {
            client.sendAndRecv();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    std::string line;
    std::cout << "> " << std::flush;
    while (std::getline(std::cin, line)) {
        if (line == "/quit") break;
        client.send(line.data(), line.size());
        client.sendAndRecv();
        std::cout << "(you) " << line << "\n> " << std::flush;
    }

    reader.detach();
    return 0;
}
