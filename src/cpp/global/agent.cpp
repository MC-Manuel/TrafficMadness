/* ***************************************************************
 *  Datei: agent.cpp
 *
 * Copyright © Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

#include <math.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <sstream>

#include "../../header/global/SInputs.hpp"
#include "../../header/global/agent.hpp"

/*
 *  Constructer
 *
 *  int states:     Anzahl Zustände welche Q-Tabele besitzt
 */
Agent::Agent(int states)
{
    this->QTable = std::vector<std::vector<float>>(
        states,
        std::vector<float>(8));

    sf::RectangleShape base;
    base.setOutlineColor(sf::Color(0, 163, 7));
    base.setOutlineThickness(2.5F);
    for (int i = 0; i < 8; i++)
    {
        base.setFillColor(i == 1 || i == 3 || i == 4 || i == 6 ? sf::Color(34, 153, 58, 100) : sf::Color(102, 255, 133, 100));
        this->statesVisuall.push_back(sf::RectangleShape(base));
    }
    this->theZone.setFillColor(sf::Color(255, 99, 92, 100));
    this->theZone.setOutlineColor(sf::Color(163, 7, 0));
    this->theZone.setOutlineThickness(2.5F);
    this->theZone.setSize(sf::Vector2f(std::stof(Input::doc->first_node("data")->first_node("agent")->first_node("enemyZone")->value()) * 2, std::stof(Input::doc->first_node("data")->first_node("agent")->first_node("enemyZone")->value()) * 2));
    this->theZone.setOrigin(this->theZone.getGlobalBounds().width / 2, this->theZone.getGlobalBounds().height / 2);

    this->font.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("infoFont")->value());
    this->overviewT.setFont(font);
    this->overviewT.setCharacterSize(20);
    this->overviewT.setFillColor(sf::Color::Red);

    this->epsilon = std::stof(Input::doc->first_node("data")->first_node("agent")->first_node("initEpsilon")->value());
}

/*
 *  Kalkuliert den Verschiebungsvektor einer ausgewählten bewegung.
 *
 *  int id:     Ausgewählte Aktion basierend auf folgende Achsen
 *       5   6   7
 *        ╲  │  ╱
 *    4 ━━      ━━ 0
 *        ╱  │  ╲
 *       3   2   1
 *  return:     Verschiebungsvektor
 */
sf::Vector2f Agent::getAction(int id)
{
    return sf::Vector2f(cos(id * -45), sin(id * -45));
}

/*
 *  Kalkuliert den Zustand (ohne Gegner)
 *
 *  Energy *goal:       Ziel des Agenten
 *  Player *player:     Spieler welcher der Agent gerade steuert
 *
 *  return:             Zustand für den gesteuerten Spieler
 */
int Agent::getState(Energy *goal, Player *player)
{
    int state = 0;

    state += isAbove(goal->getPos(), goal->sprite.getGlobalBounds()) * pow(2, 0);
    state += isBelow(goal->getPos(), goal->sprite.getGlobalBounds(), player->sprite.getGlobalBounds()) * pow(2, 1);
    state += isRight(goal->getPos(), goal->sprite.getGlobalBounds(), player->sprite.getGlobalBounds()) * pow(2, 2);
    state += isLeft(goal->getPos(), goal->sprite.getGlobalBounds()) * pow(2, 3);

    return state;
}

/*
 *  Kalkuliert den Zustand (mit Gegner)
 *
 *  Energy *goal:       Ziel des Agenten
 *  Player *player:     Spieler welcher der Agent gerade steuert
 *  List<Mot*> MOTs:    Liste aller aktiven Verkehrsmittel
 *
 *  return:             Zustand für den gesteuerten Spieler
 */
int Agent::getState(Energy *goal, Player *player, std::list<MOT *> MOTs)
{
    int state = 0;

    state += isAbove(goal->getPos(), goal->sprite.getGlobalBounds()) * pow(2, 0);
    state += isBelow(goal->getPos(), goal->sprite.getGlobalBounds(), player->sprite.getGlobalBounds()) * pow(2, 1);
    state += isRight(goal->getPos(), goal->sprite.getGlobalBounds(), player->sprite.getGlobalBounds()) * pow(2, 2);
    state += isLeft(goal->getPos(), goal->sprite.getGlobalBounds()) * pow(2, 3);

    // 0 = in range
    // 1 = above
    // 2 = below
    // 3 = right
    // 4 = left
    bool checks[] = {false, false, false, false, false};
    std::list<MOT *>::iterator m = MOTs.begin();
    while (m != MOTs.end())
    {
        MOT *mot = *m;

        if (this->inZone(mot->getPos(), mot->spriteM.getGlobalBounds(), player->sprite.getGlobalBounds()))
        {
            checks[0] = true;
        }
        if (this->isAbove(mot->getPos(), mot->spriteM.getGlobalBounds()))
        {
            checks[1] = true;
        }
        if (this->isBelow(mot->getPos(), mot->spriteM.getGlobalBounds(), player->sprite.getGlobalBounds()))
        {
            checks[2] = true;
        }
        if (this->isRight(mot->getPos(), mot->spriteM.getGlobalBounds(), player->sprite.getGlobalBounds()))
        {
            checks[3] = true;
        }
        if (this->isLeft(mot->getPos(), mot->spriteM.getGlobalBounds()))
        {
            checks[4] = true;
        }

        m++;
    }

    state += checks[0] * pow(2, 4);
    if (checks[0])
    {
        state += checks[1] * pow(2, 5);
        state += checks[2] * pow(2, 6);
        state += checks[3] * pow(2, 7);
        state += checks[4] * pow(2, 8);
    }

    return state;
}

/*
 *  Aktuallisiert die Q-Tabelle mithilfe der Bellman Formel
 *
 *  int currentState:       Zustand t
 *  int futureState:        Zustand t+1
 *  int action:             Ausgeführte Aktion
 *  int reward:             Belohnung für die ausgeführte Aktion
 */
void Agent::updateQTable(int currentState, int futureState, int action, int reward)
{
    QTable[currentState][action] = (1 - this->alpha) * QTable[currentState][action] +
                                   this->alpha * (reward + this->discountFactor * (*max_element(QTable[futureState].begin(), QTable[futureState].end())));
}

/*
 *  Konvertiert und Speichert die gerade verwendete Q-Tabelle des Agenten
 *
 *  String &buffer:     Variable in welche die konvertierte Q-Tabelle gespeichert wird
 */
void Agent::getQTable(std::string &buffer)
{
    buffer = '^';
    for (std::vector<float> i : this->QTable)
    {
        buffer += '{';
        for (float ii : i)
        {
            buffer += std::to_string(ii);
            buffer += ',';
        }
        buffer.pop_back();
        buffer += "}\n";
    }
    buffer += '^';
}

/*
 *  Konvertiert und setzt eine aus der Zwischenablage eingefügte Q-Tabelle in den Agenten ein.
 *
 *  String buffer:      Eingelesene Q-Tabelle
 *
 *  return:             Valide Eingabe
 */
bool Agent::setQTable(std::string buffer)
{
    if (buffer[0] != '^')
        return false;

    std::vector<std::vector<float>> temp;

    unsigned int openIdx = 0;
    for (int i = 1; i < buffer.length(); ++i)
    {
        if (buffer[i] == '{')
            openIdx = i;
        if (buffer[i] == '}')
        {
            // string between { }
            std::string strArr = buffer.substr(openIdx + 1, i - (openIdx + 1));

            unsigned int lastIdx = -1;
            std::vector<float> v;

            for (int ii = 0; ii < strArr.length(); ++ii)
            {
                if (strArr[ii] == ',')
                {
                    v.push_back(std::stof(strArr.substr(lastIdx + 1, ii - lastIdx)));
                    lastIdx = ii;
                }
                if (ii == strArr.length() - 1)
                {
                    v.push_back(std::stof(strArr.substr(lastIdx + 1, strArr.length() - lastIdx)));
                }
            }

            temp.push_back(v);
        }
    }

    this->QTable = temp;
    return true;
}

/*
 *  Berechnet die Belohnung für eine ausgeführte Aktion.
 *  (ohne Gegner)
 *
 *  int action:         Getätigte Aktion des Agenten
 *  Energy *goal:       Ziel des Agenten
 *  Player *player:     Gesteuerter Spieler des Agenten
 *
 *  return:             Belohnung
 */
int Agent::getReward(int action, Energy *goal, Player *player)
{
    int survival = std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("survival")->value());

    if (goal->inRange(player))
    {
        return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("collected")->value()) + survival;
    }

    float oldDis = sqrt(pow(this->oldPos.x - goal->getPos().x, 2) + pow(this->oldPos.y - goal->getPos().y, 2));
    float newDis = sqrt(pow(player->getPosition().x - goal->getPos().x, 2) + pow(player->getPosition().y - goal->getPos().y, 2));

    if (newDis < oldDis)
    {
        if (oldDis - newDis > 1.95f)
        {
            return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("strongCloser")->value()) + survival;
        }
        return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("weakCloser")->value()) + survival;
    }
    return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("notCloser")->value()) + survival;
}

/*
 *  Berechnet die Belohnung für eine ausgeführte Aktion.
 *  (mit Gegner)
 *
 *  int action:         Getätigte Aktion des Agenten
 *  Energy *goal:       Ziel des Agenten
 *  Player *player:     Gesteuerter Spieler des Agenten
 *  List<MOT*> MOTs:    Alle auf dem Feld befindenden Verkehrsmittel
 *
 *  return:             Belohnung
 */
int Agent::getReward(int action, Energy *goal, Player *player, std::list<MOT *> MOTs)
{
    int survival = std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("survival")->value());

    if (goal->inRange(player))
    {
        return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("collected")->value()) + survival;
    }

    bool zoneContains = false;
    bool closerToMOT = false;

    std::list<MOT *>::iterator m = MOTs.begin();
    while (m != MOTs.end())
    {
        MOT *mot = *m;

        if (!this->inZone(mot->getPos(), mot->spriteM.getGlobalBounds(), player->sprite.getGlobalBounds()))
        {
            ++m;
            continue;
        }
        zoneContains = true;

        if (mot->inRange(player))
        {
            return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("collided")->value()) + survival;
        }

        float oldDisMOT = sqrt(pow(this->oldPos.x - mot->getPos().x, 2) + pow(this->oldPos.y - mot->getPos().y, 2));
        float newDisMOT = sqrt(pow(player->getPosition().x - mot->getPos().x, 2) + pow(player->getPosition().y - mot->getPos().y, 2));

        if (newDisMOT < oldDisMOT)
            closerToMOT = true;

        m++;
    }

    int addReward = zoneContains
                        ? closerToMOT
                              ? std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("closerMOT")->value())
                              : std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("furtherMOT")->value())
                        : 0;
    float oldDisE = sqrt(pow(this->oldPos.x - goal->getPos().x, 2) + pow(this->oldPos.y - goal->getPos().y, 2));
    float newDisE = sqrt(pow(player->getPosition().x - goal->getPos().x, 2) + pow(player->getPosition().y - goal->getPos().y, 2));

    if (newDisE < oldDisE)
    {
        if (oldDisE - newDisE > 1.95f)
        {
            return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("strongCloser")->value()) + survival + addReward;
        }
        return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("weakCloser")->value()) + survival + addReward;
    }
    return std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("reward")->first_node("notCloser")->value()) + survival + addReward;
}

/*
 *  Aktualisiert und zeichnet die Abfragen der einzelnen Zustände.
 *  Schema:
 *    tl   │t│     tr
 *    _____│ │_______
 *   l______❏_______r
 *         │ │
 *    bl   │b│     br
 *
 *  Player *player:     Gesteuerter Spieler des Agenten
 *  bool enemies:       Ob "Complex Training" aktiviert ist. (Ob es Verkehrsmittel auf dem Feld gibt)
 *
 */
void Agent::updateVisuall(Player *player, bool enemies)
{
    int sWidth = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value());
    int sHeight = std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value());

    for (int i = 0; i < this->statesVisuall.size(); ++i)
    {
        // top
        if (i <= 2)
        {
            this->statesVisuall[i].setPosition(i == 0 ? sf::Vector2f(0, 0) : this->statesVisuall[i - 1].getPosition() + sf::Vector2f(this->statesVisuall[i - 1].getSize().x, 0));
            this->statesVisuall[i].setSize(
                sf::Vector2f(
                    i == 0
                        ? player->getPosition().x
                    : i == 1
                        ? player->sprite.getGlobalBounds().width
                        : sWidth - player->getPosition().x,
                    player->getPosition().y));
        }
        // mid
        if (i <= 4 && i >= 3)
        {
            this->statesVisuall[i].setPosition(i == 3 ? sf::Vector2f(0, player->getPosition().y) : this->statesVisuall[i - 1].getPosition() + sf::Vector2f(this->statesVisuall[i - 1].getSize().x + player->sprite.getGlobalBounds().width, 0));
            this->statesVisuall[i].setSize(sf::Vector2f(
                i == 3
                    ? player->getPosition().x
                    : sWidth - player->getPosition().x,
                player->sprite.getGlobalBounds().height));
        }
        // bottom
        if (i >= 5)
        {
            this->statesVisuall[i].setPosition(i == 5 ? sf::Vector2f(0, player->getPosition().y + player->sprite.getGlobalBounds().height) : this->statesVisuall[i - 1].getPosition() + sf::Vector2f(this->statesVisuall[i - 1].getSize().x, 0));
            this->statesVisuall[i].setSize(
                sf::Vector2f(
                    i == 5
                        ? player->getPosition().x
                    : i == 6
                        ? player->sprite.getGlobalBounds().width
                        : sWidth - player->getPosition().x,
                    sHeight - player->getPosition().y));
        }
    }

    if (enemies)
    {
        this->theZone.setPosition(player->getPosition().x + player->sprite.getGlobalBounds().width / 2, player->getPosition().y + player->sprite.getGlobalBounds().height / 2);
    }
}

/*
 *  Überprüft, ob eine angegebene Position [über] dem vom Agenten gesteuerten Spieler ist.
 *
 *  Vector2f pos:       Die zu vergleichende Position
 *  FloatRect sizeObj:  Dimensionen, des zur Position gehörenden Objektes
 */
bool Agent::isAbove(sf::Vector2f pos, sf::FloatRect sizeObj)
{
    return pos.y < this->oldPos.y && pos.y + sizeObj.height < this->oldPos.y;
}

/*
 *  Überprüft, ob eine angegebene Position [unter] dem vom Agenten gesteuerten Spieler ist.
 *
 *  Vector2f pos:       Die zu vergleichende Position
 *  FloatRect sizeObj:  Dimensionen, des zur Position gehörenden Objektes
 *  FloatRect sizeP:    Dimensionen, des von Agenten gesteuerten Spielers
 */
bool Agent::isBelow(sf::Vector2f pos, sf::FloatRect sizeObj, sf::FloatRect sizeP)
{
    return pos.y > this->oldPos.y + sizeP.height && pos.y + sizeObj.height > this->oldPos.y + sizeP.height;
}

/*
 *  Überprüft, ob eine angegebene Position auf der [rechten] Seite des vom Agenten gesteuerten Spieler ist.
 *
 *  Vector2f pos:       Die zu vergleichende Position
 *  FloatRect sizeObj:  Dimensionen, des zur Position gehörenden Objektes
 *  FloatRect sizeP:    Dimensionen, des von Agenten gesteuerten Spielers
 */
bool Agent::isRight(sf::Vector2f pos, sf::FloatRect sizeObj, sf::FloatRect sizeP)
{
    return pos.x > this->oldPos.x + sizeP.width && pos.x + sizeObj.width > this->oldPos.x + sizeP.width;
}

/*
 *  Überprüft, ob eine angegebene Position [über] dem vom Agenten gesteuerten Spieler ist.
 *
 *  Vector2f pos:       Die zu vergleichende Position
 *  FloatRect sizeObj:  Dimensionen, des zur Position gehörenden Objektes
 */
bool Agent::isLeft(sf::Vector2f pos, sf::FloatRect sizeObj)
{
    return pos.x < this->oldPos.x && pos.x + sizeObj.width < this->oldPos.x;
}

/*
 *  Überprüft, ob eine angegebene Position in der [Erkennuungszone] des vom Agenten gesteuerten Spieler ist.
 *
 *  Vector2f pos:       Die zu vergleichende Position
 *  FloatRect sizeObj:  Dimensionen, des zur Position gehörenden Objektes
 *  FloatRect sizeP:    Dimensionen, des von Agenten gesteuerten Spielers
 */
bool Agent::inZone(sf::Vector2f pos, sf::FloatRect sizeObj, sf::FloatRect sizeP)
{
    float zone = std::stof(Input::doc->first_node("data")->first_node("agent")->first_node("enemyZone")->value());
    return (pos.x <= this->oldPos.x + sizeP.width + zone && pos.x + sizeObj.width >= this->oldPos.x - zone) && (pos.y <= this->oldPos.y + sizeP.height + zone && pos.y + sizeObj.height >= this->oldPos.y - zone);
}
