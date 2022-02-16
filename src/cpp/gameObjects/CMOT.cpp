#include "../../header/gameObjects/CMOT.hpp"

#include "../../header/global/SInputs.hpp"

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
                                          : sf::Vector2f(0, 0)))); // exections bottom and right
}

void MOT::waiting()
{
    if (this->timer->getElapsedTime().asSeconds() >= std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("MOTbeforeTime")->value()))
    {
        this->tM.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("MOT")->value()); // all the path also store in xml
        this->spriteM.setTexture(this->tM);
        this->spriteM.setPosition(this->pos);

        this->spriteM.setScale(sf::Vector2f(
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("MOT")->value()),
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("MOT")->value()))); // global scale for maybe multiple screens

        this->wait = false;
        delete (this->timer);
    }
}

MOT::~MOT()
{
}

sf::Vector2f MOT::getPos()
{
    return this->pos;
}

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

bool MOT::inRange(Player *p)
{
    return (this->pos.x >= p->getPosition().x && this->pos.x <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y >= p->getPosition().y && this->pos.y <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x >= p->getPosition().x && this->pos.x <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y + this->spriteM.getGlobalBounds().height >= p->getPosition().y && this->pos.y + this->spriteM.getGlobalBounds().height <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x + this->spriteM.getGlobalBounds().width >= p->getPosition().x && this->pos.x + this->spriteM.getGlobalBounds().width <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y >= p->getPosition().y && this->pos.y <= p->getPosition().y + p->sprite.getGlobalBounds().height) ||
           (this->pos.x + this->spriteM.getGlobalBounds().width >= p->getPosition().x && this->pos.x + this->spriteM.getGlobalBounds().width <= p->getPosition().x + p->sprite.getGlobalBounds().width) &&
               (this->pos.y + this->spriteM.getGlobalBounds().height >= p->getPosition().y && this->pos.y + this->spriteM.getGlobalBounds().height <= p->getPosition().y + p->sprite.getGlobalBounds().height);
}