# 🧠 NetProbe

**NetProbe** is a lightweight C++ networking playground for exploring **sockets**, **multicast**, **TCP servers/clients**, **lock-free queues**, **custom memory pools**, and **multithreaded programming** — all wrapped with a minimal **logging and timing utility**.

It’s designed to help you understand and experiment with the **low-level building blocks** behind distributed systems, trading infrastructure, and networked applications.

---

## 📂 Directory Structure

```
./
└── src/
    ├── chat_client.cpp         # Simple TCP chat client
    ├── chat_server.cpp         # Broadcast chat server using epoll
    ├── lf_queue_example.cpp    # Lock-free queue producer-consumer demo
    ├── logging_example.cpp     # Logging utility showcase
    ├── mcast_publisher.cpp     # Multicast publisher sending random ticks
    ├── mcast_socket.cpp        # Multicast socket abstraction
    ├── mcast_subscriber.cpp    # Multicast subscriber that prints incoming ticks
    ├── mem_pool_example.cpp    # Memory pool allocator example
    ├── socket_example.cpp      # TCP server-client interaction example
    ├── tcp_server.cpp          # Epoll-based TCP server core
    ├── tcp_socket.cpp          # TCP socket abstraction
    └── thread_example.cpp      # Thread utility and example
```

---

## 🚀 Getting Started

### 🛠️ Build

Make sure you have **CMake ≥ 3.16** and a C++20 compiler.

```bash
mkdir build && cd build
cmake ..
make -j
```

This will generate all example executables inside `build/src/`.

---

## 💬 Chat Example

### 1️⃣ Start the Chat Server
```bash
./chat_server
```
Logs to: `chat_server.log`

### 2️⃣ Start one or more Clients
```bash
./chat_client
```
Each client can send messages that are broadcast to all others.  
Type `/quit` to exit.

---

## 📡 Multicast Example

### Start Subscriber
```bash
./mcast_subscriber
```

### Start Publisher
```bash
./mcast_publisher
```

The publisher sends random ticks (e.g. `TICK|AAPL|145.67`) to a multicast group (`239.192.1.1:5001`), and all subscribers receive them in real-time.

---

## 🧵 Threading & Concurrency

### Lock-Free Queue
```bash
./lf_queue_example
```
Demonstrates a **single-producer/single-consumer** lock-free queue where one thread writes and another reads data asynchronously.

### Memory Pool
```bash
./mem_pool_example
```
Custom memory pool that allocates and deallocates fixed-size objects efficiently, avoiding frequent heap allocations.

### Thread Utilities
```bash
./thread_example
```
Demonstrates spawning and managing threads using the helper `createAndStartThread()`.

---

## 🧰 Logging Example

```bash
./logging_example
```

Logs various data types to `logging_example.log` using the custom `Logger` class:
```cpp
logger.log("Logging a char:% an int:% and an unsigned:%\n", c, i, ul);
```

Each log entry is timestamped with nanosecond precision.

---

## ⚙️ Socket Example

```bash
./socket_example
```
Spins up a local TCP server and multiple clients that exchange messages through the event loop.  
Useful for understanding the flow of `sendAndRecv()`, non-blocking I/O, and epoll event handling.

---

## 🧩 Core Components Overview

| Component | Description |
|------------|-------------|
| **Logger** | Lightweight file logger with formatted log support. |
| **TCPSocket / TCPServer** | Abstractions for non-blocking TCP communication using `epoll`. |
| **McastSocket** | Multicast socket layer for market data distribution / pub-sub systems. |
| **LFQueue** | Lock-free queue for high-performance producer-consumer workloads. |
| **MemPool** | Fixed-size memory allocator for deterministic allocations. |
| **ThreadUtils** | Helper utilities for spawning and managing named threads. |

---

## 🧠 Learnings & Focus Areas

- Non-blocking I/O with `epoll`
- TCP vs Multicast communication
- Lock-free concurrency primitives
- Efficient memory management
- Logging, tracing, and profiling for network systems
- Thread orchestration and data synchronization

---

## 🪶 License

This project is licensed under the MIT License — feel free to use, modify, and share.

---

## ✨ Author

**Souvik Mukherjee**  
💼 Analyst SWE @ Zanskar Research  
🚀 Building trading infrastructure, OMS/ORMS modules, and low-latency systems in C++  
📫 [LinkedIn](https://www.linkedin.com/in/souvik150)
