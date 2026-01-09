#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Flying sunset bird");

namespace windowSize {
    float x = 800;
    float y = 600;
}

namespace sounds {
    sf::SoundBuffer jumpBuffer("jump.wav");
    sf::Sound jump(jumpBuffer);
}


void playSound(sf::Sound& sound) {
    std::cout << "volume: " << sound.getVolume() << '\n';
    sound.play();
}

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
        texture.setSmooth(true);

        scale = 2.f;
        bird.setScale({scale,scale});

        const float birdRadius = 16;
        const auto [winX, winY] = window.getSize();
        bird.setPosition({1.f * winX / 2 - scale * 16, 1.f * winY / 2 - scale * 16});
        std::cout << bird.getPosition().x << ' ' << bird.getPosition().y << '\n';

    }
    static void updateScale() {
        const float birdRadius = 16;

        const auto [winX, winY] = window.getSize();
        auto [birdX,birdY] = bird.getPosition();

        scale *= winX/windowSize::x;

        windowSize::x = winX;
        windowSize::y = winY;


        bird.setPosition({1.f * winX / 2 - scale * 16, 50});
        std::cout << bird.getPosition().x << ' ' << bird.getPosition().y << '\n';
    }
    static void jump() {
        acceleration = 9.f;
        velocity = 50.f;
        playSound(sounds::jump);
    }
    static int8_t applyPhysics(const float dt) {

        std::cout << "acceleration: " << acceleration << '\n';
        velocity += acceleration;
        if (acceleration > -8.75f) {
            acceleration -= friction;
        }


        const auto [birdX, birdY] = bird.getPosition();

        //return -1;

        bird.setPosition({birdX, (birdY - velocity * dt)});
        return 0;
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
    }

    static void over() {
        ;
    }

    static void update() {
        const float delta = clock.restart().asSeconds();
        if (status != Run) {
            return;
        }

        // returnCode:
        // -1 -> game over
        // 0  -> ok
        auto returnCode = Bird::applyPhysics(delta);
    }

};
sf::Clock Game::clock;
int8_t Game::status = Game::Stop;


int main()
{
    Bird();
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(144);
    sf::Texture backgroundTexture("background.png");
    backgroundTexture.setSmooth(true);
    sf::Sprite background(backgroundTexture,{{0,0},{1920u,1080u}});
    background.setScale({window.getSize().x/1920.f,window.getSize().y/1080.f});



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
                        if (Game::status == Game::Run) {
                            Bird::jump();
                        } else {
                            Game::status = Game::Run;
                            Bird::jump();
                        }
                        break;
                    default:
                        break;
                }
            } else if(const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                Game::status = Game::Run;
                if (click->button == sf::Mouse::Button::Left || click->button == sf::Mouse::Button::Right) {
                    if (Game::status == Game::Run) {
                        Bird::jump();
                    } else {
                        Game::status = Game::Run;
                        Bird::jump();
                    }
                }
            } else if (event->is<sf::Event::Resized>()) {
                Bird::updateScale();
                background.setScale({window.getSize().x/1920.f,window.getSize().y/1080.f});
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {windowSize::x,windowSize::y})));
            }
        }

        window.clear();
        window.draw(background);

        Game::update();

        window.draw(Bird::bird);

        window.display();
    }
}