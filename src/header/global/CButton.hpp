#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H

#include <SFML/Graphics.hpp>

enum BType
{
    small,
    medium,
    large
};

class Button
{
public:
    Button(sf::Vector2f position, char textOnButton[], bool initState, int charSize = 25, BType type = BType::medium);
    bool isOnButton(sf::Vector2f point);
    void show(sf::RenderWindow *window);

    bool enabled = true;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;

private:
    sf::Font font;
};

#endif