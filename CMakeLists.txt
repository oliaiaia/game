cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 17)

# Найти пакет SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Добавить исполняемый файл
add_executable(test main.cpp)

# Подключить необходимые компоненты SFML
target_link_libraries(test sfml-graphics sfml-window sfml-system)

