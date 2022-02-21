#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <SFML/Graphics.hpp>

#include "SInputs.hpp"
#include "agent.hpp"
#include "../gameObjects/CPlayer.hpp"
#include "../gameObjects/CEnergy.hpp"
#include "../gameObjects/CSlider.hpp"
#include "../gameObjects/CSpawner.hpp"

enum State
{
    Home = 1,
    User = 2,
    AI_NoEnemy = 3,
    AI_yesEnemy = 4,
};

class Game
{
public:
    Game(sf::RenderWindow *window, State state);
    ~Game();

    bool update(sf::RenderWindow *window, State state, Agent *agent = NULL);
    void draw(sf::RenderWindow *window);
    int getPlayerAmount();

    sf::Sprite background;

private:
    // Time
    sf::Clock timer;
    sf::Clock frame;
    sf::Clock *moveDelay = nullptr;
    // Text
    sf::Font font;
    sf::Text fps;
    sf::Text score;
    // Game Objects
    std::vector<Player *> players;
    Energy *recharger;
    Slider *energyBar;
    Spawner *spawner;

    sf::Texture t;
};

#endif