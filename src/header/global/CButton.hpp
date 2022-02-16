#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H

#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(sf::Vector2f position, char textOnButton[]);
    bool isOnButton(sf::Vector2f point);
    void show(sf::RenderWindow *window);

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;

private:
    sf::Font font;
};

#endif