#ifndef CLASS_PLAYER_H
#define CLASS_PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    //  Constructer   VELOCITY    START SCORE
    Player(float speed, int initScore);
    ~Player();

    sf::Vector2f getPosition();
    void increaseScore(unsigned int amount);
    int getScore();
    void move(sf::Vector2f vector, float dt);

    sf::Texture t;
    sf::Sprite sprite;
    sf::Clock animation;
    int source;

private:
    bool inBounds(sf::Vector2f point);
    sf::Vector2f pos;
    float speed;
    int score;
};

#endif