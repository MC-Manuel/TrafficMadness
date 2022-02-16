#include "../../header/global/CButton.hpp"
#include "../../header/global/SInputs.hpp"
#include <iostream>

Button::Button(sf::Vector2f position, char textOnButton[])
{
    this->font.loadFromFile("../assets/font.ttf");
    this->text.setFont(font);
    this->text.setFillColor(sf::Color::Black);
    this->text.setCharacterSize(20);
    this->text.setString(textOnButton);
    this->text.setOrigin(this->text.getGlobalBounds().width / 2, this->text.getGlobalBounds().height / 2);

    this->texture.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("button")->value());
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(sf::Vector2f(std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("button")->value()),
                                       std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("button")->value())));
    this->sprite.setPosition(position);
    this->text.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2, this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2));
}

void Button::show(sf::RenderWindow *window)
{
    window->draw(this->sprite);
    window->draw(this->text);
}

bool Button::isOnButton(sf::Vector2f point)
{
    float width = this->sprite.getGlobalBounds().width;
    float height = this->sprite.getGlobalBounds().height;
    sf::Vector2f topLeft = this->sprite.getPosition();

    return point.x >= topLeft.x && point.x <= topLeft.x + width && point.y >= topLeft.y && point.y <= topLeft.y + height;
}