/* ***************************************************************
 *  Datei: CEnergy.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include <time.h>

#include "../../header/global/SInputs.hpp"
#include "../../header/gameObjects/CEnergy.hpp"

/*
 *  Constructer
 *
 */
Energy::Energy()
{
    this->pos = getRandPos();

    this->texture.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("energy")->value()); // all the path also store in xml
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(this->pos);

    this->sprite.setScale(sf::Vector2f(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("energy")->value()),
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("energy")->value()))); // global scale for maybe multiple screens
}

/*
 *  Wählt eine zufällige Position auf dem Spielfeld aus
 *
 *  return:         Position
 */
sf::Vector2f Energy::getRandPos()
{

    int sWidth = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value());
    int sHeight = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value());

    return sf::Vector2f(rand() % (sWidth - (int)this->sprite.getGlobalBounds().width), rand() % (sHeight - (int)this->sprite.getGlobalBounds().height));
}

/*
 *  Überprüft ob der Spieler den Energydrink berührt hat.
 *
 *  return:         Berührung?
 */
bool Energy::inRange(Player *p)
{
    return (this->pos.x >= p->getPosition().x && this->pos.x <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y >= p->getPosition().y && this->pos.y <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x >= p->getPosition().x && this->pos.x <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y + this->sprite.getGlobalBounds().height >= p->getPosition().y && this->pos.y + this->sprite.getGlobalBounds().height <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x + this->sprite.getGlobalBounds().width >= p->getPosition().x && this->pos.x + this->sprite.getGlobalBounds().width <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y >= p->getPosition().y && this->pos.y <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x + this->sprite.getGlobalBounds().width >= p->getPosition().x && this->pos.x + this->sprite.getGlobalBounds().width <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y + this->sprite.getGlobalBounds().height >= p->getPosition().y && this->pos.y + this->sprite.getGlobalBounds().height <= p->getPosition().y + p->sprite.getGlobalBounds().height);
}

/*
 *  return:         Position
 */
sf::Vector2f Energy::getPos()
{
    return this->pos;
}