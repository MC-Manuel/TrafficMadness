#include "../../header/gameObjects/CSlider.hpp"

#include "../../header/global/SInputs.hpp"

#include <iostream>

Slider::Slider(sf::Vector2f mtBorder, float minVal, float maxVal, float initVal)
{
    this->min = minVal;
    this->max = maxVal;
    this->val = initVal;

    this->tBorder.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("slider")->first_node("border")->value());
    this->sBorder.setTexture(this->tBorder);
    this->sBorder.setOrigin(this->sBorder.getLocalBounds().width / 2, 0);
    this->sBorder.setPosition(mtBorder);
    this->sBorder.setScale(sf::Vector2f(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value()),
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value())));

    this->tFill.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("slider")->first_node("fill")->value());
    this->sFill.setTexture(this->tFill);
    this->sFill.setPosition(sf::Vector2f(mtBorder.x - (this->sBorder.getLocalBounds().width / 2 * std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value())), mtBorder.y));
    this->sFill.setScale(
        sf::Vector2f(
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value()),
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value())));
}

float Slider::getValue()
{
    return this->val;
}

void Slider::setValue(float newVal)
{
    this->val = newVal;
    this->update();
}

bool Slider::vary(float step)
{
    this->val += step;
    this->update();
    return this->val <= this->min;
}

void Slider::update()
{
    this->sFill.setScale(
        sf::Vector2f(
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value()) / this->max * this->val,
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value())));
}
