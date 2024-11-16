#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <map>

// Структура для хранения состояния игрока
struct PlayerState {
    float x, y;
};

// Глобальное состояние
std::map<int, PlayerState> playerStates;
std::mutex stateMutex; // Для синхронизации потоков

// Обработка подключения клиента
void handleClient(sf::TcpSocket* client, int playerId) {
    sf::Packet packet;
    while (client->receive(packet) == sf::Socket::Done) {
        std::string command;
        packet >> command;

        if (command == "MOVE") {
            float dx, dy;
            packet >> dx >> dy;

            // Обновление состояния игрока
            std::lock_guard<std::mutex> lock(stateMutex);
            playerStates[playerId].x += dx;
            playerStates[playerId].y += dy;
        }
        packet.clear();
    }

    // Удаление игрока при отключении
    std::lock_guard<std::mutex> lock(stateMutex);
    playerStates.erase(playerId);
    delete client;
}

int main() {
    sf::TcpListener listener;
    if (listener.listen(53000) != sf::Socket::Done) {
        std::cerr << "Ошибка запуска сервера" << std::endl;
        return -1;
    }
    std::cout << "Сервер запущен на порту 53000" << std::endl;

    int playerId = 0;
    while (true) {
        auto* client = new sf::TcpSocket;
        if (listener.accept(*client) == sf::Socket::Done) {
            {
                std::lock_guard<std::mutex> lock(stateMutex);
                playerStates[playerId] = {0.0f, 0.0f}; // Начальное положение игрока
            }
            std::thread(handleClient, client, playerId).detach();
            playerId++;
        } else {
            delete client;
        }
    }
    return 0;
}
