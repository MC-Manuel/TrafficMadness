/* ***************************************************************
 *  Datei: CPlayer.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include <iostream>

#include "../../header/global/SInputs.hpp"
#include "../../header/gameObjects/CPlayer.hpp"

/*
 *  Constructer
 *
 *  float speed:        Geschwindigkeit des Spielers
 *  int initScore:      (V0.3 @Depreciated)Startpunktezahl des Spielers
 */
Player::Player(float speed, int initScore)
{
    this->pos = sf::Vector2f(std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value()) / 2.0f,
                             std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value()) / 2.0f);
    this->speed = speed;
    this->score = initScore;

    this->t.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("player")->first_node("src")->value());
    this->sprite.setTexture(this->t);
    this->source = 0;
    this->sprite.setTextureRect(sf::IntRect(
        this->source, 0,
        std::stoi(Input::doc->first_node("data")->first_node("assets")->first_node("player")->first_node("width")->value()),
        this->sprite.getTextureRect().height));

    this->sprite.setScale(sf::Vector2f(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("player")->value()),
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("player")->value())));

    this->sprite.setPosition(this->pos);
}
/*
 *  Destructer
 *
 *  (V0.3 @Depreciated)
 */
Player::~Player()
{
}

/*
 *  Bewegt und animiert den Spieler.
 *
 *  Vector2f vector:        Benutzereingaben
 *  float dt:               Deltatime
 */
void Player::move(sf::Vector2f vector, float dt)
{
    sf::Vector2f tmpPos = this->pos;

    tmpPos.x += vector.x * this->speed * dt;
    tmpPos.y += vector.y * this->speed * dt;

    if (tmpPos == this->pos)
        return;

    if (inBounds(tmpPos))
    {
        this->pos = tmpPos;
    }
    else if (vector.x != 0 && vector.y != 0)
    {
        // if x out of bound then y can change and vise versa
        if (tmpPos.x < 0 || tmpPos.x + this->sprite.getGlobalBounds().width > std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value()))
        {
            this->pos.y = tmpPos.y;
        }
        else
        {
            this->pos.x = tmpPos.x;
        }
    }
    if (animation.getElapsedTime().asSeconds() >= 0.25f)
    {
        int width = std::stoi(Input::doc->first_node("data")->first_node("assets")->first_node("player")->first_node("width")->value());
        source = source + width >= this->t.getSize().x ? 0 : source + width;
        this->sprite.setTextureRect(sf::IntRect(
            source, 0,
            width,
            this->sprite.getTextureRect().height));
        animation.restart();
    }

    this->sprite.setPosition(this->pos);
}

/*
 *  Erhöht die Spielerpunktzahl
 *
 *  unsigned int amount:    Anzahl um die erhöht werden sollte
 *
 */
void Player::increaseScore(unsigned int amount)
{
    this->score += amount;
}

/*
 *  return:     Spielerpunktzahl
 *
 */
int Player::getScore()
{
    return this->score;
}

/*
 *  return:     Spielerposition
 *
 */
sf::Vector2f Player::getPosition()
{
    return this->pos;
}

/*
 *  Überprüft ob der Spieler sich im Spielfeld befindet
 *
 *  Vector2f point:     Position des Spielers
 *
 *  return:             Im Spielfeld?
 *
 */
bool Player::inBounds(sf::Vector2f point)
{
    int sWidth = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value());
    int sHeight = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value());

    return point.x > 0 && point.x + this->sprite.getGlobalBounds().width < sWidth && point.y > 0 && point.y + this->sprite.getGlobalBounds().height < sHeight;
}