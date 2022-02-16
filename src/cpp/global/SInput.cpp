#include "../../header/global/SInputs.hpp"

#include <math.h>

sf::Vector2f Input::coords = sf::Vector2f(0, 0);
rapidxml::xml_document<> *Input::doc = new rapidxml::xml_document<>;

void Input::look()
{
    int W = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    int A = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    int S = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    int D = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    Input::coords.x = A * -1 + D;
    Input::coords.y = W * -1 + S;

    if (Input::coords.x != 0 && Input::coords.y != 0)
    {
        Input::coords.x = Input::coords.x * std::cos(45);
        Input::coords.y = Input::coords.y * std::sin(45);
    }
}