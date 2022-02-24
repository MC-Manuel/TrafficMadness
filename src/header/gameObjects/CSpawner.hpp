/* ***************************************************************
 *  Datei: CSpawner.hpp
 *  Beschreibung:
 *      Definition der Klasse Spawner.
 *      Sie ist dafür verantwortlich die einzelnen Verkehrsmittel
 *      zu erschaffen.
 *      Zusätzlich enthält sie ein Enum für die einzelnen Himmelsrichtungen.
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#ifndef CLASS_SPAWNER_H
#define CLASS_SPAWNER_H

#include <SFML/Graphics.hpp>
#include "CMOT.hpp"

#include <string>
#include <list>
#include <vector>

enum Directions
{
    North,
    East,
    South,
    West
};

class Spawner
{
public:
    Spawner();
    ~Spawner();

    bool addMOT();
    float getDelay();

    std::list<MOT *> listMOT;

private:
    sf::Vector2f getRandPos(Directions D);
    sf::Vector2f getMultiplierVector(Directions D);
    Directions getRandDir();

    void increaseDifficulty();

    float delay;
    float MOTSpeed;
};

#endif