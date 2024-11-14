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

std::vector<sf::RectangleShape> chek_collision(std::vector<sf::RectangleShape> objects_without_collisions, sf::CircleShape main_object) {
    std::vector<two_dimensional_variable> object_parameters(objects_without_collisions.size());
    //upper left corner
    sf::Vector2f position = main_object.getPosition(); 
    //center
    sf::Vector2f center(position.x + main_object.getRadius(), position.y + main_object.getRadius()); 
    for (int t = 0; t < objects_without_collisions.size(); t++) {
        object_parameters[t].x = objects_without_collisions[t].getPosition().x + 0.5*objects_without_collisions[t].getSize().x;
        object_parameters[t].y = objects_without_collisions[t].getPosition().y + 0.5*objects_without_collisions[t].getSize().y;
    }
    for (int t = 0; t < objects_without_collisions.size(); t++) {

        if( (pow((object_parameters[t].x -  center.x), 2) + pow((object_parameters[t].y -  center.y), 2)) 
        <= pow(objects_without_collisions[t].getSize().x / 2 + main_object.getRadius(), 2)) {
            //delete object with collision
            objects_without_collisions.erase(objects_without_collisions.cbegin() + t);
        }

    }
    return objects_without_collisions;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Let's go!");
    sf::CircleShape circle(10.f); // 50.f - radius of circle
    circle.setFillColor(sf::Color::Yellow); // set circle color
    
    // sf::Texture texture;
    // if (!texture.loadFromFile("/home/olia/dino.png")) {
    // return -1;
    // }
    // sf::Sprite sprite(texture);

    circle.setPosition(0.f, 0.f); // set begin possision
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
            obstacles = chek_collision(obstacles, circle);
        }
        for (const auto &bun: chek_collision(buns, circle)) {
            window.draw(bun);
            buns = chek_collision(buns, circle);
        }
        window.draw(circle);
        window.display();
    }
    return 0;
}
