#include "../../header/global/game.hpp"

#include <iostream>
#include <algorithm>

Game::Game(sf::RenderWindow *window, State mode)
{
    srand(time(NULL));
    window->setFramerateLimit(std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("fpsCap")->value()));

    // FONT FOR FPS and SCORE
    font.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("infoFont")->value());
    fps.setFont(font);
    fps.setCharacterSize(8);
    fps.setFillColor(sf::Color::Green);
    score.setFont(font);
    score.setCharacterSize(25);
    score.setFillColor(sf::Color::Black);
    score.setOrigin(score.getLocalBounds().width / 2, 0);
    score.setPosition(
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("x")->value()) / 2,
        std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("screen")->first_node("y")->value()) * 0.095);

    if (std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("delayBetweenMoves")->value()) != 0)
    {
        moveDelay = new sf::Clock();
    }

    // Game Objects
    this->players.push_back(new Player(
        std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("pSpeed")->value()),
        0));
    if (mode != State::User)
    {
        for (int i = 0; i < std::stoi(Input::doc->first_node("data")->first_node("agent")->first_node("learners")->value()) - 1; ++i)
        {
            this->players.push_back(new Player(
                std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("pSpeed")->value()),
                0));
        }
    }

    this->recharger = new Energy();
    this->energyBar = new Slider(0, 100, 100);
    this->spawner = new Spawner();

    this->t.loadFromFile(Input::doc->first_node("data")->first_node("assets")->first_node("gameBack")->value());
    this->background.setTexture(t);
}

Game::~Game()
{
    for (int i = 0; i < this->players.size(); ++i)
    {
        delete (this->players[i]);
    }
    delete (this->recharger);
    delete (this->spawner);
    delete (this->energyBar);
}

bool Game::update(sf::RenderWindow *window, State mode, Agent *agent)
{
    float framerate = 1.0f / frame.getElapsedTime().asSeconds();
    float deltaTime = frame.restart().asSeconds();

    // Display Information
    char temp[512];
    sprintf(temp, "%f", framerate);
    fps.setString(temp);
    sprintf(temp, "%d", this->players[0]->getScore());
    score.setString(temp);

    // ....................................................................
    // Game Logic
    if (!(moveDelay == nullptr ||
          moveDelay->getElapsedTime().asSeconds() >= std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("delayBetweenMoves")->value())))
        return true;

    for (int i = 0; i < this->players.size(); ++i)
    {

        if (mode == State::User)
        {
            Input::look();
            this->players[i]->move(Input::coords, deltaTime);
        }
        else
        {

            agent->oldPos = this->players[i]->getPosition();

            int STATE = mode == State::AI_NoEnemy
                            ? agent->getState(this->recharger, this->players[i])
                            : agent->getState(this->recharger, this->players[i], this->spawner->listMOT);

            int ACTION;
            if (rand() / float(RAND_MAX) < agent->epsilon) // /distinguish between exploit and explore
            {
                // exlore
                ACTION = rand() % 8;
            }
            else
            {
                // exploit
                ACTION = std::max_element(agent->QTable[STATE].begin(), agent->QTable[STATE].end()) - agent->QTable[STATE].begin();
            }

            sf::Vector2f AIMove = agent->getAction(ACTION);

            this->players[i]->move(AIMove, deltaTime);

            int REWARD = mode == State::AI_NoEnemy
                             ? agent->getReward(ACTION, this->recharger, this->players[i])
                             : agent->getReward(ACTION, this->recharger, this->players[i], this->spawner->listMOT);

            agent->oldPos = this->players[i]->getPosition();

            int FUTURESTATE = mode == State::AI_NoEnemy
                                  ? agent->getState(this->recharger, this->players[i])
                                  : agent->getState(this->recharger, this->players[i], this->spawner->listMOT);

            agent->updateQTable(STATE, FUTURESTATE, ACTION, REWARD);
            //                                                  learning time ( should decrease over time, else the learing will be lower)

            if (this->players.size() == 1)
            {
                sprintf(temp, "Generation %d\nEpsilon: %f\nGenDropoff %f\nDicount Factor: %f\nAlpha: %f\n\nPrevious State: %d\nAction Exec: %d\nReward Earned: %d\nCurrent(next) State: %d", agent->gen, agent->epsilon, agent->genDropoff, agent->discountFactor, agent->alpha, STATE, ACTION, REWARD, FUTURESTATE);
                agent->updateVisuall(this->players[i], mode == State::AI_yesEnemy ? true : false);
            }

            // ....................................................................
        }

        // energy
        if (this->recharger->inRange(this->players[i]))
        {
            this->energyBar->setValue(100);
            this->players[i]->increaseScore((rand() % 10) + 25);
            delete (recharger);
            recharger = new Energy();
            if (mode != State::User)
            {
                if (++agent->energyCollected >= agent->maxEnergy)
                {
                    agent->energyCollected = 0;
                    return false;
                }
            }
        }

        // add MOT
        if (mode != State::AI_NoEnemy)
            if (this->timer.getElapsedTime().asSeconds() >= this->spawner->getDelay())
            {
                this->spawner->addMOT();
                this->timer.restart();
            }

        // loop though all the MOT on the field
        // maybe this need to be threaded because of efficiency
        std::list<MOT *>::iterator m = this->spawner->listMOT.begin();
        while (m != this->spawner->listMOT.end())
        {
            MOT *mot = *m;

            if (mot->inRange(this->players[i]))
            {
                return false;
            }
            ++m;
        }
    }
    // split it into two while because else if there are more than 1 learners on field they would move very fast
    std::list<MOT *>::iterator m = this->spawner->listMOT.begin();
    while (m != this->spawner->listMOT.end())
    {
        MOT *mot = *m;
        if (mot->wait)
        {
            mot->waiting();
            ++m;
            continue;
        }

        if (mot->advance(deltaTime))
        {
            // is out of bouns
            this->spawner->listMOT.erase(m++);
            continue;
        }
        ++m;
    }

    if (this->players.size() > 1)
    {
        sprintf(temp, "Generation %d\nEpsilon: %f\nGenDropoff %f\nDicount Factor: %f\nAlpha: ", agent->gen, agent->epsilon, agent->genDropoff, agent->discountFactor, agent->alpha);
        agent->overviewT.setString(temp);
        agent->overviewT.setPosition(window->getSize().x - agent->overviewT.getGlobalBounds().width, 0);
    }

    // slider
    if (this->energyBar->vary(-(std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("sliderDecay")->value()))))
    {
        return false;
    }

    if (moveDelay != nullptr)
        moveDelay->restart();

    return true;
}

void Game::draw(sf::RenderWindow *window)
{
    for (int i = 0; i < this->players.size(); ++i)
    {
        window->draw(this->players[i]->sprite);
    }

    window->draw(this->recharger->sprite);

    for (MOT *m : this->spawner->listMOT)
    {

        window->draw(m->wait ? m->spriteE : m->spriteM);
    }

    window->draw(this->fps);
    window->draw(this->score);

    window->draw(this->energyBar->sBorder);
    window->draw(this->energyBar->sFill);
}

int Game::getPlayerAmount()
{
    return this->players.size();
}