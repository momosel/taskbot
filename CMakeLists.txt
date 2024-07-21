cmake_minimum_required(VERSION 3.10)
project(RemindMeBot)

set(CMAKE_CXX_STANDARD 14)

set(TGBOT_CPP_ROOT "${CMAKE_SOURCE_DIR}/libs/tgbot-cpp")
include_directories(${TGBOT_CPP_ROOT}/include)
link_directories(${TGBOT_CPP_ROOT}/lib)

include_directories(
    ${CMAKE_SOURCE_DIR}/libs/openssl/include
    ${CMAKE_SOURCE_DIR}/libs/curl/include
    ${CMAKE_SOURCE_DIR}/libs/tgbot-cpp/include
    /usr/local/include #без локал для линукс
)

link_directories(
    ${CMAKE_SOURCE_DIR}/libs/openssl/lib
    ${CMAKE_SOURCE_DIR}/libs/curl/lib
    ${CMAKE_SOURCE_DIR}/libs/tgbot-cpp/lib
    /usr/local/lib #без локал для линукс
)

add_executable(TelegramBot TelegramBot.cpp)

target_link_libraries(TelegramBot
    TgBot
    ssl
    crypto
    curl
    boost_system
)
