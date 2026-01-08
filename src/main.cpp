#include <SFML/Graphics.hpp>
#include <iostream>

auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "CMake SFML Project");

class Bird {
private:
    static float velocity;
    static float acceleration;
    static float friction;

    static float scale;

    static sf::Texture texture;

public:
    static sf::Sprite bird;
    Bird() {
        scale = 2.f;
        bird.setScale({scale,scale});

        const float birdRadius = 16;
        const auto [winX, winY] = window.getSize();
        bird.setPosition({1.f * winX / 2 - scale * 16, 1.f * winY / 2 - scale * 16});
        std::cout << bird.getPosition().x << ' ' << bird.getTexture().getSize().y;

    }
    static void jump() {
        acceleration = 10;
        velocity = 100;
    }
    static void applyPhysics(const float dt) {

        std::cout << "acceleration: " << acceleration << '\n';
        velocity += acceleration;
        if (acceleration > -8.75f) {
            acceleration -= friction;
        }


        const auto [birdX, birdY] = bird.getPosition();
        bird.setPosition({birdX, birdY - velocity * dt});
    }
};
sf::Texture Bird::texture("berd.png",false, sf::IntRect({0,0}, {16,16}));
sf::Sprite Bird::bird(Bird::texture);
float Bird::acceleration = 0.f, Bird::friction = 0.25f, Bird::velocity = 0.f;
float Bird::scale = 0;

class Game {
private:
    static sf::Clock clock;
public:
    static int8_t status;

    enum {
        Stop = -1,
        Run  = 1,
        Over = 2
    };

    static void start() {
        status = Run;
        clock.reset();
    }

    static void update() {
        const float delta = clock.restart().asSeconds();

        Bird::applyPhysics(delta);
    }

};
sf::Clock Game::clock;
int8_t Game::status = Game::Stop;

int main()
{
    Bird();
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                Game::status = Game::Run;
                switch (key->scancode) {
                    case sf::Keyboard::Scancode::Space:
                        Bird::jump();
                        break;
                    default:
                        break;
                }
            } else if(const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                Game::status = Game::Run;
                if (click->button == sf::Mouse::Button::Left || click->button == sf::Mouse::Button::Right) {
                    Bird::jump();
                }
            }
        }

        window.clear();

        if (Game::status == Game::Run) {
            Game::update();
        }

        window.draw(Bird::bird);

        window.display();
    }
}