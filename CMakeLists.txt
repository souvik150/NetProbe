cmake_minimum_required(VERSION 3.15)
project(BuildingLowLatencyCPP LANGUAGES CXX)

# ---------------------------------------------------------
# Global settings
# ---------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

# ---------------------------------------------------------
# Common library
# ---------------------------------------------------------
add_library(common STATIC
    src/tcp_server.cpp
    src/tcp_socket.cpp
    src/mcast_socket.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(common PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# ---------------------------------------------------------
# Helper macro to add examples
# ---------------------------------------------------------
function(add_example name)
    add_executable(${name} src/${name}.cpp)
    target_link_libraries(${name} PRIVATE common)
    target_include_directories(${name} PRIVATE include)
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

# ---------------------------------------------------------
# Add all executables (from your tree)
# ---------------------------------------------------------
add_example(chat_server)
add_example(chat_client)
add_example(lf_queue_example)
add_example(logging_example)
add_example(mem_pool_example)
add_example(socket_example)
add_example(thread_example)
add_example(mcast_publisher)
add_example(mcast_subscriber)

# ---------------------------------------------------------
# Optional install step
# ---------------------------------------------------------
install(TARGETS common
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
