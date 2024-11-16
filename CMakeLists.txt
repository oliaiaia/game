cmake_minimum_required(VERSION 3.10)
project(test)

set(CMAKE_CXX_STANDARD 17)

# Найти пакет SFML
find_package(SFML 2.5 COMPONENTS network graphics window system REQUIRED)
find_package(nlohmann_json REQUIRED)


# Добавить исполняемый файл
add_executable(test main.cpp)
add_executable(server socketServer.cpp)
add_executable(client socketClient.cpp)

# Подключить необходимые компоненты SFML
target_link_libraries(test sfml-graphics sfml-window sfml-system)
target_link_libraries(server sfml-network sfml-system sfml-graphics sfml-window)
target_link_libraries(client sfml-network sfml-system sfml-graphics sfml-window)

