#include <SFML/Network.hpp> // Подключение библиотеки SFML для работы с сетью
#include <iostream>         // Для вывода сообщений в консоль
#include <thread>           // Для работы с потоками
#include <vector>           // Для использования контейнера std::vector (здесь не используется)
#include <mutex>            // Для синхронизации потоков
#include <map>              // Для хранения состояний игроков

// Структура для хранения состояния игрока
struct PlayerState {
    float x, y; // Координаты игрока
};

// Глобальное состояние игроков
std::map<int, PlayerState> playerStates; // Карта (словарь), где каждому игроку соответствует его состояние
std::mutex stateMutex; // Мьютекс для защиты общей структуры от одновременного доступа из нескольких потоков

// Функция для обработки подключения клиента
void handleClient(sf::TcpSocket* client, int playerId) {
    sf::Packet packet; // Пакет для получения данных от клиента
    while (client->receive(packet) == sf::Socket::Done) { // Пока успешно принимаются данные от клиента
        std::string command;
        packet >> command; // Чтение команды из пакета

        if (command == "MOVE") { // Если команда - перемещение игрока
            float dx, dy;
            packet >> dx >> dy; // Чтение значений изменения координат

            // Обновление состояния игрока
            std::lock_guard<std::mutex> lock(stateMutex); // Защита playerStates мьютексом
            playerStates[playerId].x += dx; // Обновление координаты x
            playerStates[playerId].y += dy; // Обновление координаты y
        }
        packet.clear(); // Очистка пакета для следующего использования
    }

    // Удаление игрока при отключении
    std::lock_guard<std::mutex> lock(stateMutex); // Защита playerStates при удалении записи
    playerStates.erase(playerId); // Удаление состояния игрока из карты
    delete client; // Освобождение памяти, выделенной для сокета
}

int main() {
    sf::TcpListener listener; // Слушатель для входящих подключений
    if (listener.listen(53000) != sf::Socket::Done) { // Запуск сервера на порту 53000
        std::cerr << "Ошибка запуска сервера" << std::endl; // Сообщение об ошибке
        return -1; // Завершение программы с кодом ошибки
    }
    std::cout << "Сервер запущен на порту 53000" << std::endl; // Уведомление о запуске сервера

    int playerId = 0; // Уникальный идентификатор для каждого игрока
    while (true) { // Главный цикл сервера
        auto* client = new sf::TcpSocket; // Создание нового сокета для клиента
        if (listener.accept(*client) == sf::Socket::Done) { // Ожидание подключения клиента
            {
                std::lock_guard<std::mutex> lock(stateMutex); // Защита playerStates
                playerStates[playerId] = {0.0f, 0.0f}; // Установка начального состояния игрока
            }
            std::thread(handleClient, client, playerId).detach(); // Запуск нового потока для обработки клиента
            playerId++; // Увеличение идентификатора игрока
        } else {
            delete client; // Освобождение памяти, если подключение не удалось
        }
    }
    return 0; // Завершение программы
}
