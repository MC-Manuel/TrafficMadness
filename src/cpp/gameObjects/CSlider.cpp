/* ***************************************************************
 *  Datei: CSlider.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include <iostream>
#include <math.h>

#include "../../header/gameObjects/CSlider.hpp"
#include "../../header/global/SInputs.hpp"

/*
 *  Constructer
 *
 *  float minVal:       Wert bei welchem der Slider eine 0% Breite hat
 *  float maxVal:       Wert bei welchem der Slider eine 100% Breite hat
 *  float initVal:      Startwert des Sliders
 */
Slider::Slider(float minVal, float maxVal, float initVal)
{
    this->min = minVal;
    this->max = maxVal;
    this->val = initVal;

    this->tBorder.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("slider")->first_node("border")->value());
    this->sBorder.setTexture(this->tBorder);
    this->sBorder.setScale(sf::Vector2f(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value()),
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value())));

    this->sBorder.setOrigin(this->sBorder.getLocalBounds().width / 2, this->sBorder.getLocalBounds().height / 2);
    this->sBorder.setPosition(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value()) / 2,
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value()) * 0.055);

    this->tFill.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("slider")->first_node("fill")->value());
    this->sFill.setTexture(this->tFill);
    this->rectWidth = this->sFill.getTextureRect().width;
    this->sFill.setScale(
        sf::Vector2f(
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value()) * 0.975,
            std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("scaleFactor")->first_node("slider")->value()) * 0.975));
    this->sFill.setOrigin(0, this->sFill.getLocalBounds().height / 2);
    this->sFill.setPosition(
        this->sBorder.getPosition().x - (this->sBorder.getGlobalBounds().width / 2) * 0.975,
        this->sBorder.getPosition().y);
}

/*
 *  return:     Aktueller Wert
 *
 */
float Slider::getValue()
{
    return this->val;
}

/*
 *  Legt einen neuen Wert fest.
 *
 *  float newVal:       Neuer Wert
 */
void Slider::setValue(float newVal)
{
    this->val = newVal;
    this->update();
}

/*
 *  Verändert den Wert um [Step] Einheiten.
 *
 *  float step:     Veränderung
 *
 *  return:         Minimum erreicht?
 */
bool Slider::vary(float step)
{
    this->val += step;
    this->update();
    return this->val <= this->min;
}

/*
 *  Aktualisiert die Anzeige des Sliders. Setzt Maximum und aktuellen Wert in ein Verhältniss.
 *
 */
void Slider::update()
{
    this->sFill.setTextureRect(
        sf::IntRect(
            0,
            0,
            (int)(this->rectWidth * (((100.0f / this->max) * this->val) / 100.0f)),
            this->sFill.getTextureRect().height));
}
