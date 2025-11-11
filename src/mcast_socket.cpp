#include "mcast_socket.h"

#include <unistd.h>     // for close()
#include <sys/socket.h> // for send(), recv()
#include <cstring>      // for memcpy()

namespace Common {

// =============================
// Impl definition (hidden data)
// =============================
struct McastSocket::Impl {
    int socket_fd_ = -1;
    std::vector<char> outbound_data_;
    size_t next_send_valid_index_ = 0;
    std::vector<char> inbound_data_;
    size_t next_rcv_valid_index_ = 0;
    std::function<void(McastSocket*)> recv_callback_ = nullptr;
    std::string time_str_;
    Logger& logger_;

    explicit Impl(Logger& logger) : logger_(logger) {
        outbound_data_.resize(McastBufferSize);
        inbound_data_.resize(McastBufferSize);
    }

    ~Impl() {
        if (socket_fd_ != -1)
            close(socket_fd_);
    }
};

// =============================
// Public interface forwarding
// =============================
McastSocket::McastSocket(Logger& logger)
    : impl_(std::make_unique<Impl>(logger)) {}

McastSocket::~McastSocket() = default;
McastSocket::McastSocket(McastSocket&&) noexcept = default;
McastSocket& McastSocket::operator=(McastSocket&&) noexcept = default;

// ------------------------------------------------------------
// Initialize multicast socket to read from or publish to stream
// ------------------------------------------------------------
auto McastSocket::init(const std::string& ip,
                       const std::string& iface,
                       int port,
                       bool is_listening) -> int {
    const SocketCfg socket_cfg{ip, iface, port, true, is_listening, false};
    impl_->socket_fd_ = createSocket(impl_->logger_, socket_cfg);
    return impl_->socket_fd_;
}

// ------------------------------------------------------------
// Join multicast group
// ------------------------------------------------------------
bool McastSocket::join(const std::string& ip) {
    return Common::join(impl_->socket_fd_, ip);
}

// ------------------------------------------------------------
// Leave multicast group
// ------------------------------------------------------------
auto McastSocket::leave(const std::string&, int) -> void {
    if (impl_->socket_fd_ != -1) {
        close(impl_->socket_fd_);
        impl_->socket_fd_ = -1;
    }
}

// ------------------------------------------------------------
// Publish outgoing data and read incoming data
// ------------------------------------------------------------
auto McastSocket::sendAndRecv() noexcept -> bool {
    const ssize_t n_rcv = recv(
        impl_->socket_fd_,
        impl_->inbound_data_.data() + impl_->next_rcv_valid_index_,
        McastBufferSize - impl_->next_rcv_valid_index_,
        MSG_DONTWAIT
    );

    if (n_rcv > 0) {
        impl_->next_rcv_valid_index_ += n_rcv;
        impl_->logger_.log("%:% %() % read socket:% len:%\n",
            __FILE__, __LINE__, __FUNCTION__,
            Common::getCurrentTimeStr(&impl_->time_str_),
            impl_->socket_fd_,
            impl_->next_rcv_valid_index_);

        if (impl_->recv_callback_)
            impl_->recv_callback_(this);
    }

    if (impl_->next_send_valid_index_ > 0) {
        ssize_t n = ::send(
            impl_->socket_fd_,
            impl_->outbound_data_.data(),
            impl_->next_send_valid_index_,
            MSG_DONTWAIT | MSG_NOSIGNAL
        );

        impl_->logger_.log("%:% %() % send socket:% len:%\n",
            __FILE__, __LINE__, __FUNCTION__,
            Common::getCurrentTimeStr(&impl_->time_str_),
            impl_->socket_fd_, n);
    }

    impl_->next_send_valid_index_ = 0;
    return (n_rcv > 0);
}

// ------------------------------------------------------------
// Copy data to send buffer (does not send yet)
// ------------------------------------------------------------
auto McastSocket::send(const void* data, size_t len) noexcept -> void {
    std::memcpy(impl_->outbound_data_.data() + impl_->next_send_valid_index_, data, len);
    impl_->next_send_valid_index_ += len;
    ASSERT(impl_->next_send_valid_index_ < McastBufferSize,
           "Mcast socket buffer filled up and sendAndRecv() not called.");
}

// ------------------------------------------------------------
// Register callback for received data
// ------------------------------------------------------------
void McastSocket::setRecvCallback(std::function<void(McastSocket*)> cb) noexcept {
    impl_->recv_callback_ = std::move(cb);
}


size_t McastSocket::recvSize() const noexcept {
    return impl_->next_rcv_valid_index_;
}

void McastSocket::resetRecvSize() noexcept {
    impl_->next_rcv_valid_index_ = 0;
}

// ------------------------------------------------------------
// Accessors
// ------------------------------------------------------------
int McastSocket::fd() const noexcept { return impl_->socket_fd_; }
std::vector<char>& McastSocket::outboundBuffer() noexcept { return impl_->outbound_data_; }
std::vector<char>& McastSocket::inboundBuffer() noexcept { return impl_->inbound_data_; }
Logger& McastSocket::logger() noexcept { return impl_->logger_; }

} // namespace Common
