#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <fstream>
#include <math.h>

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
        "SA Projekt", sf::Style::Close | sf::Style::Close | sf::Style::Titlebar);

    Game *game = nullptr;

    State gameState = State::Home;

    sf::Texture t;
    t.loadFromFile("../assets/preGameCover.png");
    sf::Sprite cover;
    cover.setTexture(t);

    sf::Text titel;
    sf::Font f;
    f.loadFromFile("../assets/font.ttf");
    titel.setFont(f);
    titel.setString("Traffic Madness");
    titel.setCharacterSize(50);
    titel.setOrigin(titel.getGlobalBounds().width / 2, titel.getGlobalBounds().height / 2);
    titel.setPosition(window.getSize().x / 2, window.getSize().y * 0.1);

    Button *buttonPlayer = new Button(sf::Vector2f(200, 200), "Player");
    Button *buttonAINE = new Button(sf::Vector2f(400, 200), "AI (no Enemy)");
    Button *buttonAIE = new Button(sf::Vector2f(600, 200), "Ai (with Enemy)");

    Button *inQTable = new Button(sf::Vector2f(500, 300), "Input\nQTable");
    Button *outQTable = new Button(sf::Vector2f(500, 300), "Output\nQTable");
    Button *goOn = new Button(sf::Vector2f(500, 500), "Continue");

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
                        agent->epsilon = agent->epsilon + agent->colDropoff <= 1 ? agent->epsilon + agent->colDropoff : 1;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        agent->epsilon = agent->epsilon - agent->colDropoff >= 0 ? agent->epsilon - agent->colDropoff : 0;
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
                        if (gameState == State::User)
                        {
                            game = new Game(&window);
                        }
                        else
                        {
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
                            game = new Game(&window);
                        preGame = false;
                        afterGame = false;
                    }
                }
            }
        }

        // GAME UPDATE
        if (game != nullptr)
        {
            if (gameState != State::User)
            {
                if (agent->restart)
                {
                    delete (game);
                    game = new Game(&window);
                    agent->gen++;
                    agent->epsilon -= agent->epsilon / 100 * agent->genDropoff;
                    agent->restart = false;
                }
            }

            if (!game->update(&window, gameState, gameState == State::User ? nullptr : agent))
            {
                if (gameState == State::User)
                {
                    delete (game);
                    game = nullptr;
                    std::cout << "oki";
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
            if (indicators && gameState != State::User)
            {
                window.draw(agent->overviewT);
                for (sf::RectangleShape rect : agent->statesVisuall)
                {
                    window.draw(rect);
                }
                if (gameState == State::AI_yesEnemy)
                    window.draw(agent->theZone);
            }

            game->draw(&window);
        }
        else
        {
            window.draw(cover);
            if (preGame)
            {
                inQTable->show(&window);
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
            }
        }

        window.display();
    }

    return 0;
}
