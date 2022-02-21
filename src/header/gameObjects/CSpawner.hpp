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

    bool addMOT(); // could give id, but for now this function chooses randomly
    float getDelay();

    std::list<MOT *> listMOT;

private:
    sf::Vector2f getRandPos(Directions D);
    sf::Vector2f getMultiplierVector(Directions D);
    Directions getRandDir();

    void increaseDifficulty();

    float delay; // in seconds
    float MOTSpeed;
};

#endif