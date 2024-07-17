cmake_minimum_required(VERSION 3.10)
project(RemindMeBot)

set(CMAKE_CXX_STANDARD 14)

# Указываем пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/libs/openssl/include
    ${CMAKE_SOURCE_DIR}/libs/curl/include
    ${CMAKE_SOURCE_DIR}/libs/tgbot-cpp/include
    /usr/local/include  # Путь к заголовкам Boost
)

# Указываем пути к библиотекам
link_directories(
    ${CMAKE_SOURCE_DIR}/libs/openssl/lib
    ${CMAKE_SOURCE_DIR}/libs/curl/lib
    ${CMAKE_SOURCE_DIR}/libs/tgbot-cpp/lib
    /usr/local/lib  # Путь к библиотекам Boost
)

# Добавляем исполняемый файл
add_executable(TelegramBot TelegramBot.cpp)

# Связываем библиотеки
target_link_libraries(TelegramBot
    TgBot
    ssl
    crypto
    curl
    boost_system  # Убедитесь, что эта библиотека доступна
)
