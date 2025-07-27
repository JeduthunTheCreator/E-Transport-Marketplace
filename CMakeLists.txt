cmake_minimum_required(VERSION 3.21)

project(dashboard)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

find_package(SQLite3 REQUIRED)

add_executable(dashboard src/main.cpp include/Dashboard.h src/Dashboard.cpp src/CargoOwner.cpp src/Driver.cpp src/LoginLogout.cpp src/SocketConnection.cpp src/TransportCompany.cpp src/main.cpp include/main.h)

target_link_libraries(dashboard PRIVATE SQLite::SQLite3)
