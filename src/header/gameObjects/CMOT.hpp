/* ***************************************************************
 *  Datei: CMOT.hpp
 *  Beschreibung:
 *      Definition der Klasse MOT.
 *      Beinhaltet die Bewegung des Vekehrsmittel                   [advance()],
 *      die Abfrage obder Spieler in Berührung gekommen ist         [inRange()]
 *      und die Andeutung das bald ein Verkehrsmittel gespawnt wird [waiting()] .
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#ifndef CLASS_MOT_H
#define CLASS_MOT_H

#include <SFML/Graphics.hpp>
#include <string>

#include "CPlayer.hpp"
// MEANS OF TRAFFIC
class MOT
{
public:
    MOT(sf::Vector2f startP, sf::Vector2f multP, float speed);
    ~MOT();

    sf::Vector2f getPos();

    sf::Texture tM;
    sf::Sprite spriteM;
    sf::Texture tE;
    sf::Sprite spriteE;
    bool advance(float deltaTime); // goes one step in dirct, returns wheter out of bounds
    bool inRange(Player *p);
    void waiting();

    bool wait;
    sf::Clock *timer;

private:
    sf::Vector2f pos;
    sf::Vector2f dirct;

    float speed;
    float radius = 25.0f;
};

#endif