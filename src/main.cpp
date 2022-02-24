/* ***************************************************************
 *  Datei: main.cpp
 *  Beschreibung:
 *      Der Eingangspunkt der Applikation.
 *
 * Copyright (c) Manuel Capeder, Traffic Madness, 25.02.2022
 *************************************************************** */

// Include
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <fstream>
#include <math.h>

// Header
#include "header/global/game.hpp"
#include "header/global/CButton.hpp"
#include "external/rapidxml.hpp"

int main()
{
    // LOAD XML
    std::ifstream myfile("../settings.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    Input::doc->parse<0>(&buffer[0]);

    // CREATE WINDOW
    sf::RenderWindow window(
        sf::VideoMode(
            std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value()),
            std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value())),
        "Traffic Madness", sf::Style::Close | sf::Style::Close | sf::Style::Titlebar);

    Game *game = nullptr;

    State gameState = State::Home;

    sf::Texture tB;
    tB.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("background")->value());
    sf::Sprite cover(tB);

    sf::Texture tI;
    tI.loadFromFile("../assets/MENU/HomeLogo.png");
    sf::Sprite logo(tI);
    logo.setScale(0.5, 0.5);
    logo.setPosition(500, 200);

    sf::Text titel;
    sf::Font f;
    f.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("titelFont")->value());
    titel.setFont(f);
    titel.setString("Traffic Madness");
    titel.setCharacterSize(75);
    titel.setOrigin(titel.getGlobalBounds().width / 2, titel.getGlobalBounds().height / 2);
    titel.setPosition(window.getSize().x / 2, window.getSize().y * 0.1);

    Button *buttonPlayer = new Button(sf::Vector2f(150, 200), "Play", true, 40);
    Button *buttonAINE = new Button(sf::Vector2f(150, 375), "Train Simple", true, 30);
    Button *buttonAIE = new Button(sf::Vector2f(150, 550), "Train Complex", true, 30);

    Button *inQTable = new Button(sf::Vector2f(500, 300), "Import\nQTable", false, 35);
    Button *outQTable = new Button(sf::Vector2f(500, 300), "Export\nQTable", false, 35);
    Button *goHome = new Button(sf::Vector2f(50, 600), "Home", false, 40, BType::small);
    Button *goOn = new Button(sf::Vector2f(500, 500), "Continue", false, 40);

    Agent *agent = nullptr;

    bool indicators = true;
    bool preGame = false;
    bool afterGame = false;

    std::string bufferQTable = "";

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) // game stop
                {
                    if (game != nullptr)
                    {
                        if (gameState != State::User)
                        {
                            afterGame = true;
                            // convet QTable to string for Clipboard
                            agent->getQTable(bufferQTable);

                            outQTable->enabled = true;
                            goOn->enabled = true;
                        }
                        else
                        {
                            buttonPlayer->enabled = true;
                            buttonAIE->enabled = true;
                            buttonAINE->enabled = true;
                        }
                        delete (game);
                        delete (agent);
                        game = nullptr;
                        agent = nullptr;

                        gameState = State::Home;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) // game stop
                {
                    indicators = !indicators;
                }

                if (gameState != State::User)
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        agent->epsilon = agent->epsilon + 0.01f <= 1 ? agent->epsilon + 0.01f : 1;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        agent->epsilon = agent->epsilon - 0.01f >= 0 ? agent->epsilon - 0.01f : 0;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (game == nullptr && !afterGame)
                {
                    if (buttonPlayer->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        gameState = State::User;
                    }
                    if (buttonAINE->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        gameState = State::AI_NoEnemy;
                        agent = new Agent((int)pow(2, 4));
                    }
                    if (buttonAIE->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        gameState = State::AI_yesEnemy;
                        agent = new Agent((int)pow(2, 9));
                    }

                    if (gameState != State::Home)
                    {
                        buttonPlayer->enabled = false;
                        buttonAIE->enabled = false;
                        buttonAINE->enabled = false;

                        if (gameState == State::User)
                        {
                            game = new Game(&window, gameState);
                        }
                        else
                        {
                            inQTable->enabled = true;
                            goHome->enabled = true;
                            goOn->enabled = true;
                            preGame = true;
                        }
                    }
                }
                if (preGame)
                {
                    if (inQTable->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        sf::String str = sf::Clipboard::getString();
                        std::string bufferQTable = str.toAnsiString();
                        agent->setQTable(bufferQTable);
                    }
                    if (goHome->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        inQTable->enabled = false;
                        goHome->enabled = false;
                        goOn->enabled = false;
                        preGame = false;
                    }
                }
                if (afterGame)
                {
                    if (outQTable->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        sf::Clipboard::setString(bufferQTable);
                    }
                }
                if (preGame || afterGame)
                {
                    if (goOn->isOnButton((sf::Vector2f)sf::Mouse::getPosition(window)))
                    {
                        if (preGame)
                        {
                            game = new Game(&window, gameState);
                            preGame = false;
                            inQTable->enabled = false;
                            goHome->enabled = false;
                            goOn->enabled = false;
                        }
                        if (afterGame)
                        {
                            afterGame = false;
                            outQTable->enabled = false;
                            goOn->enabled = false;
                            buttonPlayer->enabled = true;
                            buttonAIE->enabled = true;
                            buttonAINE->enabled = true;
                        }
                    }
                }
            }
        }

        // GAME UPDATE
        if (game != nullptr)
        {
            if (gameState != State::User && gameState != State::Home)
            {
                if (agent->restart)
                {
                    delete (game);
                    game = new Game(&window, gameState);
                    agent->gen++;
                    agent->epsilon *= pow(agent->genDropoff, game->getPlayerAmount());
                    agent->restart = false;
                    agent->energyCollected = 0;
                }
            }

            if (!game->update(&window, gameState, gameState == State::User ? nullptr : agent))
            {
                if (gameState == State::User)
                {
                    gameState = State::Home;
                    buttonPlayer->enabled = true;
                    buttonAIE->enabled = true;
                    buttonAINE->enabled = true;
                    delete (game);
                    game = nullptr;
                }
                else
                {
                    agent->restart = true;
                }
            }
        }

        // GAME DISPLAY
        window.clear(sf::Color::White);
        if (game != nullptr)
        {
            window.draw(game->background);

            if (indicators && gameState != State::User)
            {
                window.draw(agent->overviewT);
                if (game->getPlayerAmount() == 1)
                {
                    for (sf::RectangleShape rect : agent->statesVisuall)
                    {
                        window.draw(rect);
                    }
                    if (gameState == State::AI_yesEnemy)
                        window.draw(agent->theZone);
                }
            }

            game->draw(&window);
        }
        else
        {
            window.draw(cover);
            if (preGame)
            {
                inQTable->show(&window);
                goHome->show(&window);
                goOn->show(&window);
            }
            else if (afterGame)
            {
                outQTable->show(&window);
                goOn->show(&window);
            }
            else
            {
                buttonPlayer->show(&window);
                buttonAINE->show(&window);
                buttonAIE->show(&window);
                window.draw(titel);
                window.draw(logo);
            }
        }

        window.display();
    }

    return 0;
}
