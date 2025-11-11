# ===========================================
# Simple Makefile wrapper around CMake build
# ===========================================

# Build directories
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib

# -------------------------------------------
# Default target: configure & build
# -------------------------------------------
.PHONY: all
all:
	@echo "🔧 Configuring and building with CMake..."
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) --parallel

# -------------------------------------------
# Run examples / demos
# -------------------------------------------
.PHONY: run-socket
run-socket:
	@$(BIN_DIR)/socket_example

.PHONY: run-logging
run-logging:
	@$(BIN_DIR)/logging_example

.PHONY: run-thread
run-thread:
	@$(BIN_DIR)/thread_example

.PHONY: run-mem-pool
run-mem-pool:
	@$(BIN_DIR)/mem_pool_example

.PHONY: run-lf-queue
run-lf-queue:
	@$(BIN_DIR)/lf_queue_example

.PHONY: chat-server
chat-server:
	@echo "🚀 Starting Chat Server..."
	@$(BIN_DIR)/chat_server

.PHONY: chat-client
chat-client:
	@echo "💬 Starting Chat Client..."
	@$(BIN_DIR)/chat_client

.PHONY: mcast-publisher
mcast-publisher:
	@echo "📡 Starting Multicast Publisher..."
	@$(BIN_DIR)/mcast_publisher

.PHONY: mcast-subscriber
mcast-subscriber:
	@echo "📻 Starting Multicast Subscriber..."
	@$(BIN_DIR)/mcast_subscriber

# -------------------------------------------
# Clean / Rebuild targets
# -------------------------------------------
.PHONY: clean
clean:
	@echo "🧹 Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

.PHONY: rebuild
rebuild: clean all

# -------------------------------------------
# Install targets (optional)
# -------------------------------------------
.PHONY: install
install:
	@cmake --install $(BUILD_DIR)

# -------------------------------------------
# Utility shortcuts
# -------------------------------------------
.PHONY: list
list:
	@echo "Available run targets:"
	@echo "  make run-socket"
	@echo "  make run-logging"
	@echo "  make run-thread"
	@echo "  make run-mem-pool"
	@echo "  make run-lf-queue"
	@echo "  make chat-server"
	@echo "  make chat-client"
	@echo "  make mcast-publisher"
	@echo "  make mcast-subscriber"
