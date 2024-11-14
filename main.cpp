#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
using namespace std;
#include <vector>
#include <cmath>

const int screen_width = 1800;
const int screen_height = 1024;

std::vector<sf::RectangleShape> create_obstacles() {
    srand(time(0));
    int amount_obstacles = rand() % 20 + 5;
    std::vector<sf::RectangleShape> obstacles(amount_obstacles);
    for (int t = 0; t < amount_obstacles; t++) {
        sf::RectangleShape rectangle(sf::Vector2f(10.f, 10.f)); // Создаем прямоугольник
        rectangle.setFillColor(sf::Color::Red); // Устанавливаем цвет

        // Устанавливаем позицию для каждого прямоугольника
        rectangle.setPosition(1.f * (rand() % screen_height), 1.f * (rand() % screen_width));
        // Пример: размещаем в ряд с интервалом 50 пикселей

        // Сохраняем прямоугольник в вектор
        obstacles[t] = rectangle;
    }
    return obstacles;
}

std::vector<sf::RectangleShape> create_buns() {
    srand(time(0));
    int amount_buns = rand() % 20 + 5;
    std::vector<sf::RectangleShape> buns(amount_buns);
    for (int t = 0; t < amount_buns; t++) {
        sf::RectangleShape rectangle(sf::Vector2f(10.f, 10.f)); // Создаем прямоугольник
        rectangle.setFillColor(sf::Color::Green); // Устанавливаем цвет

        // Устанавливаем позицию для каждого прямоугольника
        rectangle.setPosition(1.f * ((rand() + 150) % screen_height), 1.f * ((rand() + 150) % screen_width));
        // Пример: размещаем в ряд с интервалом 50 пикселей

        // Сохраняем прямоугольник в вектор
        buns[t] = rectangle;
    }
    return buns;
}

struct two_dimensional_variable{
    double x;
    double y;
};

std::vector<sf::RectangleShape> chek_collision(std::vector<sf::RectangleShape> collisions_objects, sf::CircleShape main_object) {
    std::vector<two_dimensional_variable> object_parameters(collisions_objects.size());
    sf::Vector2f position = main_object.getPosition(); // верхний левый угол круга
    sf::Vector2f center(position.x + main_object.getRadius(), position.y + main_object.getRadius()); // центр круга
    for (int t = 0; t < collisions_objects.size(); t++) {

        object_parameters[t].x = collisions_objects[t].getPosition().x + 0.5*collisions_objects[t].getSize().x;
    //cout << "size: " << object_parameters[t].x << endl;
        object_parameters[t].y = collisions_objects[t].getPosition().y + 0.5*collisions_objects[t].getSize().y;
    //cout << "size: " << object_parameters[t].y << endl;
    }

    // прописать отслеживание столкновений
    for (int t = 0; t < collisions_objects.size(); t++) {
        //cout << (pow((object_parameters[t].x -  center.x), 2) + pow((object_parameters[t].y -  center.y), 2)) << endl;
        //cout << pow(collisions_objects[t].getSize().x / 2 + main_object.getRadius(), 2) << endl;

        if( (pow((object_parameters[t].x -  center.x), 2) + pow((object_parameters[t].y -  center.y), 2)) 
        <= pow(collisions_objects[t].getSize().x / 2 + main_object.getRadius(), 2)) {
            cout << "TRUE" << endl;
            collisions_objects.erase(collisions_objects.cbegin() + t);
        }
        // else             cout << "FALSE" << endl;

    }
    return collisions_objects;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Let's go!");
    sf::CircleShape circle(10.f); // 50.f - радиус круга
    circle.setFillColor(sf::Color::Yellow); // Устанавливаем цвет круга
    // sf::Texture texture;
    // if (!texture.loadFromFile("/home/olia/dino.png")) {
    // return -1;
    // }
    // sf::Sprite sprite(texture);
    circle.setPosition(0.f, 0.f); // Устанавливаем начальную позицию спрайта

    std::vector<sf::RectangleShape> obstacles = create_obstacles();
    std::vector<sf::RectangleShape> buns = create_buns();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            circle.move(-0.1f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            circle.move(0.1f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            circle.move(0.f, -0.1f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            circle.move(0.f, 0.1f);

        window.clear();

        for (const auto &obstacle: chek_collision(obstacles, circle)) {
            window.draw(obstacle);
        }
        for (const auto &bun: chek_collision(buns, circle)) {
            window.draw(bun);
        }
        window.draw(circle);
        window.display();
    }

    return 0;
}
