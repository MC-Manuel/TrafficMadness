#ifndef CLASS_PLAYER_H
#define CLASS_PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    //  Constructer         SPAWNPOINT    VELOCITY    START SCORE
    Player(sf::Vector2f spawnPos, float speed, int initScore);
    ~Player();

    sf::Vector2f getPosition();
    void increaseScore(unsigned int amount);
    int getScore();
    bool move(sf::Vector2f vector, float dt);

    sf::Texture t;
    sf::Sprite sprite;

private:
    bool inBounds(sf::Vector2f point);
    sf::Vector2f pos;
    float speed;
    int score;
};

#endif