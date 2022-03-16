/* ***************************************************************
 *  Datei: CButton.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include <iostream>

#include "../../header/global/CButton.hpp"
#include "../../header/global/SInputs.hpp"

/*
 *  Constructor
 *
 *  Vector position:        Position (Top Left)
 *  char textOnButton[]:    Button Inhalt
 *  bool initState:         Aktiviert oder deaktiviert
 *  int charSize:           Schrifftgrösse
 *  BType type:             Type, definiert die Grösse
 *
 */
Button::Button(sf::Vector2f position, char textOnButton[], bool initState, int charSize, BType type)
{
    this->font.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("buttonFont")->value());
    this->text.setFont(font);
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(charSize);
    this->text.setString(textOnButton);
    this->text.setOrigin(this->text.getGlobalBounds().width / 2, this->text.getGlobalBounds().height / 2);

    this->texture.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("button")->value());
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(
        sf::Vector2f(
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("button")->value()) * (0.75f + (type * 0.25f)),
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("button")->value())) *
        (0.75f + (type * 0.25f)));
    this->sprite.setPosition(position);
    this->text.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2, this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2));

    this->enabled = initState;
}

/*
 *  Zeichnet den Knopf auf den Bildschirm
 *
 *  RenderWindow *window:       Feld auf welches gezeichnet wird
 */
void Button::show(sf::RenderWindow *window)
{
    if (this->succ)
    {
        sf::Texture tS;
        tS.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("success")->value());
        sf::Sprite successful(tS);
        successful.setScale(this->sprite.getScale());
        successful.setPosition(this->sprite.getPosition());
        window->draw(successful);
    }
    else
    {
        window->draw(this->sprite);
        window->draw(this->text);
    }
}

/*
 *  Überprüft ein mitgegebener Punkt auf dem Knopf ist.
 *
 *  Vector2f point:         Mausposition
 *
 *  return:                 Auf Knopf?
 */
bool Button::isOnButton(sf::Vector2f point)
{
    if (!enabled)
        return false;
    float width = this->sprite.getGlobalBounds().width;
    float height = this->sprite.getGlobalBounds().height;
    sf::Vector2f topLeft = this->sprite.getPosition();

    return point.x >= topLeft.x && point.x <= topLeft.x + width && point.y >= topLeft.y && point.y <= topLeft.y + height;
}