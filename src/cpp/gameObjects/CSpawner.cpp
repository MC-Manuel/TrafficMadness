/* ***************************************************************
 *  Datei: CSpawner.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include "../../header/gameObjects/CSpawner.hpp"
#include <time.h>
#include <math.h>
#include <iostream>
#include "../../header/global/SInputs.hpp"

/*
 *  Constructer
 *
 */
Spawner::Spawner()
{
    this->delay = std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("baseMOTdelay")->value());
    this->MOTSpeed = std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("baseMOTSpeed")->value());
}

/*
 *  Destructer
 *
 *  Befreit jeglichen zugeordneten Speicherplatz vom Speicher.
 */
Spawner::~Spawner()
{
    for (auto i : this->listMOT)
    {
        delete (i);
    }
}

/*
 *  Erschafft ein neues Verkehrsmittel.
 *
 *  return:         (V0.1, @depreciated)Limit der Fahrzeuge erreicht
 */
bool Spawner::addMOT()
{
    Directions tmpDir = this->getRandDir();
    this->listMOT.push_back(
        new MOT(this->getRandPos(tmpDir),
                this->getMultiplierVector(tmpDir), this->MOTSpeed));
    this->increaseDifficulty();
    return true;
}

/*
 *
 *  Erhöht den Schwierigkeit grad des Spieles.
 *  Verkehrsmittel sollen sich schneller bewegen und erscheinen.
 *
 */
void Spawner::increaseDifficulty()
{
    float increase = std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("diffIncrease")->value()); // in %
    this->MOTSpeed += this->MOTSpeed >= std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("maxMOTSpeed")->value()) ? 0 : this->MOTSpeed * (increase / 100);
    this->delay -= this->delay <= std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("minDelay")->value()) ? 0 : this->delay * (increase / 100);
}

/*
 *
 *  return:     Delay
 *
 */
float Spawner::getDelay()
{
    return this->delay;
}

/*
 *  Wählt eine Zufällige Randposition aus.
 *
 *  Direction D:        An welcher Wand die Zufallsposition entschieden werden soll.
 *
 *  return:             Randposition
 */
sf::Vector2f Spawner::getRandPos(Directions D)
{
    int sWidth = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value());
    int sHeight = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value());

    switch (D)
    {
    case North:
        return sf::Vector2f(rand() % sWidth, sHeight);
    case East:
        return sf::Vector2f(0, rand() % sHeight);
    case South:
        return sf::Vector2f(rand() % sWidth, 0);
    case West:
        return sf::Vector2f(sWidth, rand() % sHeight);
    }

    return sf::Vector2f(-1, -1);
}

/*
 *  Wählt eine Zufällige Randposition aus.
 *
 *  Direction D:        Für welche Wand
 *
 *  return:             Bewegungsrichtung
 */
sf::Vector2f Spawner::getMultiplierVector(Directions D)
{
    switch (D)
    {
    case North:
        return sf::Vector2f(0, -1);
    case East:
        return sf::Vector2f(1, 0);
    case South:
        return sf::Vector2f(0, 1);
    case West:
        return sf::Vector2f(-1, 0);
    }

    return sf::Vector2f(0, 0);
}

/*
 *  Returniert eine Zufällige Himmelsrichtung
 *
 *  return:             Himmelsrichtung
 */
Directions Spawner::getRandDir()
{

    switch (rand() % 4 + 1)
    {
    case 1:
        return Directions::North;
    case 2:
        return Directions::South;
    case 3:
        return Directions::East;
    case 4:
        return Directions::West;
    default:
        return Directions::North;
    }
}
