/* ***************************************************************
 *  Datei: CButton.hpp
 *  Beschreibung:
 *      Definition der Klasse Button.
 *      Sie kann global in allen Klassen genutzt werden.
 *      Das Herzstück dieser Klasse mach die [isOnButton()]
 *      Methode aus. Sie überprüft ob ein mitgegebener Punkt auf dem
 *      Knopf ist.
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

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