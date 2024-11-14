#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;



const int screen_width = 1280;
const int screen_height = 720;


void resetGame(sf::CircleShape &circle, std::vector<sf::RectangleShape> &obstacles, std::vector<sf::RectangleShape> &buns);


std::vector<sf::RectangleShape> create_obstacles() {
    srand(time(0));
    int amount_obstacles = rand() % 20 + 5;
    std::vector<sf::RectangleShape> obstacles(amount_obstacles);
    for (int t = 0; t < amount_obstacles; t++) {
        sf::RectangleShape rectangle(sf::Vector2f(10.f, 10.f)); 
        rectangle.setFillColor(sf::Color::Red); 
        rectangle.setPosition(1.f * (rand() % screen_width), 1.f * (rand() % screen_height));
        obstacles[t] = rectangle;
    }
    return obstacles;
}

std::vector<sf::RectangleShape> create_buns() {
    srand(time(0));
    int amount_buns = rand() % 20 + 5;
    std::vector<sf::RectangleShape> buns(amount_buns);
    for (int t = 0; t < amount_buns; t++) {
        sf::RectangleShape rectangle(sf::Vector2f(10.f, 10.f)); // create rectangle
        rectangle.setFillColor(sf::Color::Green); // set color

        // set posses for each rectangle
        rectangle.setPosition(1.f * ((rand() + 150) % screen_width ), 1.f * ((rand() + 150) % screen_height));
        // save rectangle  to vector
        buns[t] = rectangle;
    }
    return buns;
}

struct two_dimensional_variable{
    double x;
    double y;
};

struct collision{
    std::vector<sf::RectangleShape> objects_without_collisions;
    bool flag;
};

collision chek_collision(std::vector<sf::RectangleShape> objects_without_collisions, sf::CircleShape main_object) {
    
    collision current_object;
    current_object.flag = false;
    current_object.objects_without_collisions = objects_without_collisions;

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
            current_object.flag = true;
            current_object.objects_without_collisions = objects_without_collisions;
            return current_object;
        }

    }

    return current_object;
}

int main() {

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Let's go!");

    sf::CircleShape circle(10.f); // 50.f - radius of circle
    circle.setFillColor(sf::Color::Yellow); // set circle color

    //settings for window game over
    sf::Texture game_over;
    if (!game_over.loadFromFile("../game_over.png")) {
        return -1;
    }
    sf::Sprite sprite(game_over);
    
    sf::Texture win;
    if (!win.loadFromFile("../win.png")) {
        return -1;
    }
    sf::Sprite spriteWin(win);

    sf::Texture buttons;
    if (!buttons.loadFromFile("../press.png")) {
        return -1;
    }
    sf::Sprite spritebuttons(buttons);
    

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u game_overSize = game_over.getSize();
    sf::Vector2u winSize = win.getSize();
    

    // finding center possesions
    float xPosGameOver = (windowSize.x - game_overSize.x) / 2.0f;
    float yPosGameOver = (windowSize.y - game_overSize.y) / 2.0f;
    sprite.setPosition(xPosGameOver, yPosGameOver);
    
    float xPosWin= (windowSize.x - winSize.x) / 2.0f;
    float yPosWin = (windowSize.y - winSize.y) / 2.0f;
    spriteWin.setPosition(xPosWin, yPosWin);

    spriteWin.setPosition(0.f, 0.f);
    
    circle.setPosition(0.f, 0.f); // set begin possision
    std::vector<sf::RectangleShape> obstacles = create_obstacles();
    std::vector<sf::RectangleShape> buns = create_buns();


    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        char direction_circle_move;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            circle.move(-0.1f, 0.f);
            direction_circle_move = 'l';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            circle.move(0.1f, 0.f);
            direction_circle_move = 'r';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            circle.move(0.f, -0.1f);
            direction_circle_move = 'u';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            circle.move(0.f, 0.1f);
            direction_circle_move = 'd';
        }

        window.clear();

        auto last_radius = circle.getRadius();

        //obstacles
        collision current_state_obstacles = chek_collision(obstacles, circle);

        for (const auto &obstacle: obstacles) {
            window.draw(obstacle);

            if(current_state_obstacles.flag) {
                switch (direction_circle_move)
                {
                case 'l':
                    circle.move(0.5f, 0.f);
                    break;
                case 'r':
                    circle.move(-0.5f, 0.f);
                    break;
                case 'u':
                    circle.move(0.f, 0.5f);
                    break;
                case 'd':
                    circle.move(0.f, -0.5f);
                    break;
                default:
                    break;
                }

                circle.setRadius(last_radius - 1.0);
            }

        }

        //buns
        auto last_size_buns = buns.size();

        collision current_state_buns = chek_collision(buns, circle);
        buns = current_state_buns.objects_without_collisions;

        for (const auto &bun: current_state_buns.objects_without_collisions) {

            if(current_state_buns.flag) {
                circle.setRadius(last_radius + 2.0);
            }

            window.draw(bun);
        }

        if (buns.empty()) {
            window.draw(spriteWin);
            window.draw(spritebuttons);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                std::cout << "Enter is pressed - Restarting the game" << std::endl;
                resetGame(circle, obstacles, buns); // Restart game
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                std::cout << "Esc is pressed - Closing the game" << std::endl;
                window.close(); // Close window
            }
        } else if (last_radius <= 0.0) {
            window.draw(sprite);
            window.draw(spritebuttons);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                std::cout << "Enter is pressed - Restarting the game" << std::endl;
                resetGame(circle, obstacles, buns); // Restart game
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                std::cout << "Esc is pressed - Closing the game" << std::endl;
                window.close(); // Close window 
            }
        } else {
            window.draw(circle); // Draw the circle (player) if the game continues
        }

        window.display();

    }
    return 0;
}


void resetGame(sf::CircleShape &circle, std::vector<sf::RectangleShape> &obstacles, std::vector<sf::RectangleShape> &buns) {
    circle.setPosition(0.f, 0.f);
    circle.setRadius(10.f);
    obstacles = create_obstacles(); // Create new obstacles
    buns = create_buns(); // Create new "buns"  
}

