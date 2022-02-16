#ifndef STATIC_INPUTS_H
#define STATIC_INPUTS_H

#include <SFML/Graphics.hpp>

#include "../../external/rapidxml.hpp"

class Input
{
public:
    static sf::Vector2f coords;
    static void look();
    static rapidxml::xml_document<> *doc;
};

#endif