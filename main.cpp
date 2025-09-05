#include <iostream>
#include <SFML/Graphics.hpp>
#include "snake.h"
#include <random>

using namespace std;
random_device rd;
mt19937 gerador(rd());
sf::Vector2f random_food_pos(Snake& player) {
    uniform_int_distribution<int> range(0, 11);

    float x = range(gerador) * 50.f;
    float y = range(gerador) * 50.f;
    sf::Vector2f coords(x, y);
    while (!player.is_food_safe(coords)) {
        x = range(gerador) * 50.f;
        y = range(gerador) * 50.f;
        coords.x = x;
        coords.y = y;
    }

    return coords;
    
}

bool coliding_wall(float axis1, sf::Vector2f borders) {
    return axis1 >= borders.y || axis1 < borders.x;
}


int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "SnakeGame");
    window.setFramerateLimit(8);

    sf::RectangleShape head(sf::Vector2f(50.f, 50.f));
    head.setFillColor(sf::Color::Red);
    head.setPosition(50.f, 50.f);

    Snake player = Snake(head.getPosition());

    sf::RectangleShape food(sf::Vector2f(40.f, 40.f));
    food.setFillColor(sf::Color::Magenta);
    food.setPosition(random_food_pos(player));

    //sf::Vector2f vel(0.f, 0.f);
    //sf::Clock clk;
    float xvel = 0;
    float yvel = 0;
    bool lost = false;
    bool win = false;
    int direction = 0; //1/-1 = Norte/Sul | 2/-2 = Leste/Oeste | 0 = Parado
    while (window.isOpen()) {
        //float dt = clk.restart().asSeconds();
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }

            if (e.type == sf::Event::KeyPressed) {
                if (!lost && !win) {
                    if (e.key.code == sf::Keyboard::W && direction != 1) {
                        xvel = 0.f;
                        yvel = -50.f;
                        direction = -1;
                    }

                    if (e.key.code == sf::Keyboard::S && direction != -1) {
                        xvel = 0.f;
                        yvel = 50.f;
                        direction = 1;
                    }

                    if (e.key.code == sf::Keyboard::A && direction != -2) {
                        xvel = -50.f;
                        yvel = 0.f;
                        direction = 2;
                    }

                    if (e.key.code == sf::Keyboard::D && direction != 2) {
                        xvel = 50.f;
                        yvel = 0.f;
                        direction = -2;
                    }
                } 
                if (lost || win) { 
                    if (e.key.code == sf::Keyboard::R) {
                        xvel = 0;
                        yvel = 0;
                        direction = 0;
                        head.setPosition(50.f, 50.f);
                        food.setPosition(random_food_pos(player));
                        player.reset();
                        lost = false;
                        win = false;
                    }
                }
            }
        }

        if (coliding(head.getPosition(), food.getPosition())) {
            player.increase_size(food.getPosition());
            food.setPosition(random_food_pos(player));
        }

        if (coliding_wall(head.getPosition().x, sf::Vector2f(0.0f, 600.0f)) || coliding_wall(head.getPosition().y, sf::Vector2f(0.0f, 600.0f))) {
            lost = true;
        }

        if (player.get_size() == 12 * 12) {
            win = true;
        }

        if (lost) {
            sf::Text game_over;

            sf::Font game_over_font;
            game_over_font.loadFromFile("./assets/font.ttf");

            game_over.setFont(game_over_font);
            game_over.setString("Game Over!");
            game_over.setCharacterSize(36);
            game_over.setFillColor(sf::Color::White);

            game_over.setPosition(100.f, 250.f);
            window.clear(sf::Color::Black);
            window.draw(game_over);
        } else if (win) {
            sf::Text win_screen;

            sf::Font win_screen_font;
            win_screen_font.loadFromFile("./assets/font.ttf");

            win_screen.setFont(win_screen_font);
            win_screen.setString("You Win!");
            win_screen.setCharacterSize(36);
            win_screen.setFillColor(sf::Color::Green);

            win_screen.setPosition(100.f, 250.f);
            window.clear(sf::Color::Black);
            window.draw(win_screen);

        } else {
            head.move(xvel, yvel);
            player.move(head.getPosition());
            if (player.is_safe()) {
                window.clear(sf::Color::Black);
                Node* current = player.get_head()->next;
                while (current != NULL) {
                    sf::RectangleShape body(sf::Vector2f(50.f, 50.f));
                    body.setFillColor(sf::Color::Green);
                    body.setPosition(current->position);
                    window.draw(body);
                    current = current->next;
                }

                window.draw(food);
                window.draw(head);
            } else {
                lost = true;
                xvel = 0;
                yvel = 0;
            }
        }

        window.display();
    }

}