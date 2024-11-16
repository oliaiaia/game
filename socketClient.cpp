#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

// Глобальное состояние игры
float playerX = 0.0f, playerY = 0.0f;

void receiveUpdates(sf::TcpSocket& socket) {
    sf::Packet packet;
    while (socket.receive(packet) == sf::Socket::Done) {
        packet >> playerX >> playerY; // Получаем обновлённое состояние
        packet.clear();
    }
}

int main() {
    sf::TcpSocket socket;

    if (socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        return -1;
    }

    std::thread receiver(receiveUpdates, std::ref(socket));
    receiver.detach();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Клиент");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Отправляем команды движения
        sf::Packet packet;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            packet << "MOVE" << -1.0f << 0.0f;
            socket.send(packet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            packet << "MOVE" << 1.0f << 0.0f;
            socket.send(packet);
        }

        // Отрисовка игрока
        window.clear();
        sf::CircleShape player(10.0f);
        player.setPosition(playerX, playerY);
        window.draw(player);
        window.display();
    }

    return 0;
}
