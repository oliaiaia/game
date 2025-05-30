#include <SFML/Network.hpp>  // Для работы с сетью
#include <SFML/Graphics.hpp> // Для графического отображения
#include <iostream>          // Для вывода в консоль
#include <thread>            // Для работы с потоками

// Глобальное состояние игры: координаты игрока
float playerX = 0.0f, playerY = 0.0f;

// Функция для получения обновлений от сервера
void receiveUpdates(sf::TcpSocket& socket) {
    sf::Packet packet; // Пакет для получения данных от сервера
    while (socket.receive(packet) == sf::Socket::Done) { // Пока успешно принимаются данные
        packet >> playerX >> playerY; // Извлечение координат игрока из пакета
        packet.clear(); // Очистка пакета для повторного использования
    }
}

int main() {
    sf::TcpSocket socket; // Сокет для подключения к серверу

    // Подключение к серверу на локальной машине (127.0.0.1) на порту 53000
    if (socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
        std::cerr << "Ошибка подключения к серверу" << std::endl; // Сообщение об ошибке подключения
        return -1; // Завершение программы с кодом ошибки
    }

    // Запуск потока для получения обновлений от сервера
    std::thread receiver(receiveUpdates, std::ref(socket)); // Поток получает обновления, передача сокета по ссылке
    receiver.detach(); // Отсоединяем поток, чтобы он работал независимо

    // Создание окна для графического отображения
    sf::RenderWindow window(sf::VideoMode(800, 600), "Клиент");

    // Главный цикл приложения
    while (window.isOpen()) {
        sf::Event event; // Событие для обработки пользовательского ввода
        while (window.pollEvent(event)) { // Проверка всех событий в очереди
            if (event.type == sf::Event::Closed) // Если событие — закрытие окна
                window.close(); // Закрыть окно
        }

        // Отправка команды движения на сервер
        sf::Packet packet; // Пакет для отправки данных
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { // Если нажата клавиша "влево"
            packet << "MOVE" << -1.0f << 0.0f; // Команда перемещения влево
            socket.send(packet); // Отправка пакета серверу
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { // Если нажата клавиша "вправо"
            packet << "MOVE" << 1.0f << 0.0f; // Команда перемещения вправо
            socket.send(packet); // Отправка пакета серверу
        }

        // Отрисовка текущего состояния игрока
        window.clear(); // Очистка экрана перед рисованием
        sf::CircleShape player(10.0f); // Создание игрока в виде круга радиусом 10 пикселей
        player.setPosition(playerX, playerY); // Установка позиции игрока на экране
        window.draw(player); // Отрисовка игрока
        window.display(); // Обновление содержимого окна
    }

    return 0; // Завершение программы
}
