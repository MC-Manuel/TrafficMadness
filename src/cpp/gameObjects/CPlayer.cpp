#include "../../header/gameObjects/CPlayer.hpp"

#include <iostream>

#include "../../header/global/SInputs.hpp"

Player::Player(sf::Vector2f spawnPos, float speed, int initScore)
{
    this->pos = spawnPos;
    this->speed = speed;
    this->score = initScore;

    // sprite
    this->t.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("player")->value());
    this->sprite.setTexture(Player::t);
    this->sprite.setPosition(Player::pos);
    this->sprite.setScale(sf::Vector2f(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("player")->value()),
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("player")->value()))); // global scale would be nice
}

Player::~Player()
{
    // score save or something
}

bool Player::move(sf::Vector2f vector, float dt)
{
    sf::Vector2f tmpPos = this->pos;

    tmpPos.x += vector.x * this->speed * dt;
    tmpPos.y += vector.y * this->speed * dt;

    if (inBounds(tmpPos))
    {
        this->pos = tmpPos;
        this->sprite.setPosition(this->pos);
        return true;
    }
    return false;
}

void Player::increaseScore(unsigned int amount)
{
    this->score += amount;
}

int Player::getScore()
{
    return this->score;
}

sf::Vector2f Player::getPosition()
{
    return this->pos;
}

bool Player::inBounds(sf::Vector2f point)
{
    int sWidth = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value());
    int sHeight = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value());

    // nur oben links bis jetzt

    return point.x > 0 && point.x + this->sprite.getGlobalBounds().width < sWidth && point.y > 0 && point.y + this->sprite.getGlobalBounds().height < sHeight;
}