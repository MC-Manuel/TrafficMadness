#ifndef CLASS_SLIDER_H
#define CLASS_SLIDER_H

#include <SFML/Graphics.hpp>

class Slider
{
public:
    Slider(float minVal, float maxVal, float initVal = 0.0f);

    float getValue();
    void setValue(float newVal);
    bool vary(float step);

    sf::Texture tBorder;
    sf::Texture tFill;
    sf::Sprite sBorder;
    sf::Sprite sFill;

private:
    void update();
    sf::Vector2f pos;
    float max;
    float min;
    float val;

    int rectWidth;
};

#endif