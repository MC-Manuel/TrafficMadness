#ifndef CLASS_ENERGY_H
#define CLASS_ENERGY_H

#include <SFML/Graphics.hpp>
#include "CPlayer.hpp"

class Energy
{
public:
    Energy();

    bool inRange(Player *p);
    sf::Vector2f getPos();

    sf::Texture texture;
    sf::Sprite sprite;

private:
    sf::Vector2f getRandPos();

    sf::Vector2f pos;
};

#endif