#include "tcp_server.h"
#include "logging.h"
#include "time_utils.h"

using namespace Common;

int main() {
    Logger logger("chat_server.log");

    TCPServer server(logger);
    const std::string iface = "lo";
    const int port = 5555;

    logger.log("🚀 Starting ChatServer on iface:% port:%\n", iface, port);
    server.listen(iface, port);

    // When any client sends data
    server.recv_callback_ = [&](TCPSocket* socket, Nanos rx_time) noexcept {
        std::string msg(socket->inbound_data_.data(), socket->next_rcv_valid_index_);
        socket->next_rcv_valid_index_ = 0;

        // Log received message
        logger.log("📩 recv from fd:% msg:%\n", socket->socket_fd_, msg);

        // Build message to broadcast
        std::string broadcast_msg = "[Client " + std::to_string(socket->socket_fd_) + "]: " + msg;

        // Broadcast to everyone
        for (auto* s : server.send_sockets_) {
            s->send(broadcast_msg.data(), broadcast_msg.size());
            s->sendAndRecv();
        }
    };

    server.recv_finished_callback_ = [&]() noexcept {
        server.sendAndRecv();
    };

    while (true) {
        server.poll();
        server.sendAndRecv();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
