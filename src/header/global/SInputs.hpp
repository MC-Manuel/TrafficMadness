/* ***************************************************************
 *  Datei: SInput.hpp
 *  Beschreibung:
 *      Definition der statischen Klasse Input.
 *      Prüft jeden Frame die Nutzereingaben und hat die XML Datei
 *      [settings.xml] gespeichert für Globalen gebrauch.
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */
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