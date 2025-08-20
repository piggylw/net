cmake_minimum_required(VERSION 3.10)
project(testlogger)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(testlogger
    testlogger.cpp
    dblogger/src/logger.cpp
    dblogger/src/formatter.cpp
    dblogger/src/logsink.cpp
)

target_include_directories(testlogger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/dblogger/include
)