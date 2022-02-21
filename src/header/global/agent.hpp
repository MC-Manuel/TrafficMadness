#ifndef AGENT_H
#define AGENT_H

#include <SFML/Graphics.hpp>

#include "../../header/global/SInputs.hpp"
#include "../../header/gameObjects/CEnergy.hpp"
#include "../../header/gameObjects/CPlayer.hpp"
#include "../../header/gameObjects/CMOT.hpp"

#include <random>
#include <iostream>
#include <list>
#include <vector>

class Agent
{
public:
    Agent(int states);

    // --- QLearning ------

    sf::Vector2f getAction(int id);
    int getState(Energy *goal, Player *player);
    int getState(Energy *goal, Player *player, std::list<MOT *>); // with enemies
    int getReward(int action, Energy *goal, Player *player);
    int getReward(int action, Energy *goal, Player *player, std::list<MOT *> MOTs); // with enemies

    void updateQTable(int currentState, int futureState, int action, int reward);
    void getQTable(std::string &buffer);
    bool setQTable(std::string buffer);

    std::vector<std::vector<float>> QTable;

    int gen = 0;
    float genDropoff = 0.995;
    float epsilon = 1.0f; // likelyhood of explore
    float discountFactor = 0.9f;
    float alpha = 0.15f;
    int maxEnergy = 10; // so the agent won't go on forever
    int energyCollected = 0;
    // ----------------------

    bool restart = false;

    sf::Vector2f oldPos; // reference to current position

    // ---------------------

    sf::Font font;
    sf::Text overviewT;

    // ---------------------
    sf::RectangleShape theZone;
    std::vector<sf::RectangleShape> statesVisuall;
    void updateVisuall(Player *player, bool enemies = false);

private:
    bool isAbove(sf::Vector2f pos, sf::FloatRect sizeObj);
    bool isBelow(sf::Vector2f pos, sf::FloatRect sizeObj, sf::FloatRect sizeP);
    bool isRight(sf::Vector2f pos, sf::FloatRect sizeObj, sf::FloatRect sizeP);
    bool isLeft(sf::Vector2f pos, sf::FloatRect sizeObj);
    bool inZone(sf::Vector2f pos, sf::FloatRect sizeObj, sf::FloatRect sizeP);
};

#endif