/* ***************************************************************
 *  Datei: CMOT.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include "../../header/gameObjects/CMOT.hpp"
#include "../../header/global/SInputs.hpp"

/*
 *  Constructer
 *
 *  Vector2f startP:    Startposition
 *  Vector2f multP:     Fahrtrichtung
 *  float speed:        Geschwindigkeit
 */
MOT::MOT(sf::Vector2f startP, sf::Vector2f multP, float speed)
{

    this->pos = startP;
    this->dirct = multP;
    this->speed = speed;

    this->wait = true;
    timer = new sf::Clock();

    this->tE.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("MOTbefore")->value());
    this->spriteE.setTexture(this->tE);
    this->spriteE.setScale(sf::Vector2f(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("MOT")->value()),
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("MOT")->value())));
    this->spriteE.setPosition(startP -
                              (this->dirct.x == -1
                                   ? sf::Vector2f(this->spriteE.getGlobalBounds().width, 0)
                                   : (this->dirct.y == -1
                                          ? sf::Vector2f(0, this->spriteE.getGlobalBounds().height)
                                          : sf::Vector2f(0, 0)))); // exceptions bottom and right
}

/*
 *  Destructer
 *
 */
MOT::~MOT()
{
}

/*
 *  Wartet eine benutzerdefiniert Zeit bevor das Verkehrsmittel erschaffen wird. Während dieser
 *  Zeit wird ein Ausrufezeichen angezeigt.
 *
 */
void MOT::waiting()
{
    if (this->timer->getElapsedTime().asSeconds() >= std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("MOTbeforeTime")->value()))
    {
        char *options[] = {"truck",
                           "car",
                           "bus",
                           "bycl"};
        this->tM.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("MOTS")->first_node(options[rand() % 4])->value()); // all the path also store in xml
        this->spriteM.setTexture(this->tM);
        this->spriteM.setPosition(this->pos);

        this->spriteM.setScale(sf::Vector2f(
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("MOT")->value()),
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("MOT")->value())));

        this->wait = false;
        delete (this->timer);
    }
}

/*
 *  return:     Position
 *
 */
sf::Vector2f MOT::getPos()
{
    return this->pos;
}

/*
 *  Bewegt den Spieler in Fahrtrichtung.
 *
 *  float dt:       Deltatime
 *
 *  return:         Kollidiert mit einer Wand?
 */
bool MOT::advance(float dt)
{
    this->pos.x += this->dirct.x * this->speed * dt;
    this->pos.y += this->dirct.y * this->speed * dt;

    this->spriteM.setPosition(this->pos);

    int sWidth = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value());
    int sHeight = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value());

    if (this->pos.x < 0 || this->pos.x > sWidth || this->pos.y < 0 || this->pos.y > sHeight)
        return true;
    return false;
}

/*
 *  Überprüft ob der Spieler [p] das Verkehrsmittel berührt hat.
 *
 *  Player *p:      Spieler
 *
 *  return:         Kollision?
 */
bool MOT::inRange(Player *p)
{
    // Kann mit Matheformel vereinfacht werden
    return (this->pos.x >= p->getPosition().x && this->pos.x <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y >= p->getPosition().y && this->pos.y <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x >= p->getPosition().x && this->pos.x <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y + this->spriteM.getGlobalBounds().height >= p->getPosition().y && this->pos.y + this->spriteM.getGlobalBounds().height <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x + this->spriteM.getGlobalBounds().width >= p->getPosition().x && this->pos.x + this->spriteM.getGlobalBounds().width <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y >= p->getPosition().y && this->pos.y <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x + this->spriteM.getGlobalBounds().width >= p->getPosition().x && this->pos.x + this->spriteM.getGlobalBounds().width <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y + this->spriteM.getGlobalBounds().height >= p->getPosition().y && this->pos.y + this->spriteM.getGlobalBounds().height <= p->getPosition().y + p->sprite.getGlobalBounds().height);
}