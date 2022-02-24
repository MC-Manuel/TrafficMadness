/* ***************************************************************
 *  Datei: CEnergy.hpp
 *  Beschreibung:
 *      Definition der Klasse Energy.
 *      Sie umfasss die zufällige Positionsberechnung und
 *      überprüft, ob der Spieler in der nähe des Energydrinks ist.
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */
#ifndef CLASS_ENERGY_H
#define CLASS_ENERGY_H

#include <SFML/Graphics.hpp>
#include "CPlayer.hpp"

class Energy
{
public:
    Energy();

    bool inRange(Player *p);
    sf::Vector2f getPos();

    sf::Texture texture;
    sf::Sprite sprite;

private:
    sf::Vector2f getRandPos();

    sf::Vector2f pos;
};

#endif