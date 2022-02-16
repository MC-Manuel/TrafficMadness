#include "../../header/global/game.hpp"

#include <iostream>
#include <algorithm>

Game::Game(sf::RenderWindow *window)
{
    srand(time(NULL));
    window->setFramerateLimit(std::stoi(Input::doc->first_node("data")->first_node("meta")->first_node("fpsCap")->value()));

    // FONT FOR FPS and SCORE
    font.loadFromFile("../assets/font.ttf");
    fps.setFont(font);
    fps.setCharacterSize(8);
    fps.setFillColor(sf::Color::Green);
    score.setFont(font);
    score.setCharacterSize(25);
    score.setFillColor(sf::Color::Black);
    score.setOrigin(score.getGlobalBounds().width / 2, 0);
    score.setPosition(window->getSize().x / 2, window->getSize().y * 0.095);

    if (std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("delayBetweenMoves")->value()) != 0)
    {
        moveDelay = new sf::Clock();
    }

    // Game Objects
    this->player = new Player(
        sf::Vector2f(
            window->getSize().x / 2,
            window->getSize().y / 2),
        std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("pSpeed")->value()),
        0);
    this->recharger = new Energy();
    this->energyBar = new Slider(sf::Vector2f(window->getSize().x / 2, window->getSize().y * 0.01), 0, 100, 100);
    this->spawner = new Spawner();
}

Game::~Game()
{
    delete (this->player);
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
    sprintf(temp, "%d", this->player->getScore());
    score.setString(temp);

    // ....................................................................
    // Game Logic
    if (!(moveDelay == nullptr ||
          moveDelay->getElapsedTime().asSeconds() >= std::stof(Input::doc->first_node("data")->first_node("meta")->first_node("delayBetweenMoves")->value())))
        return true;

    if (mode == State::User)
    {
        Input::look();
        this->player->move(Input::coords, deltaTime);
    }
    else
    {
        // MAYBE WAIT FOR A CERTAIN AMOUNT OF TIME SO THE AGENT DOESNT START ANYWHERE
        agent->oldPos = this->player->getPosition();

        int STATE = mode == State::AI_NoEnemy
                        ? agent->getState(this->recharger, this->player)
                        : agent->getState(this->recharger, this->player, this->spawner->listMOT);

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

        this->player->move(AIMove, deltaTime);

        int REWARD = mode == State::AI_NoEnemy
                         ? agent->getReward(ACTION, this->recharger, this->player)
                         : agent->getReward(ACTION, this->recharger, this->player, this->spawner->listMOT);

        agent->oldPos = this->player->getPosition();

        int FUTURESTATE = mode == State::AI_NoEnemy
                              ? agent->getState(this->recharger, this->player)
                              : agent->getState(this->recharger, this->player, this->spawner->listMOT);

        agent->updateQTable(STATE, FUTURESTATE, ACTION, REWARD);
        //                                                  learning time ( should decrease over time, else the learing will be lower)

        sprintf(temp, "Generation %d\nEpsilon: %f\nGenDropoff %f\nColDropoff %f\nDicount Factor: %f\nAlpha: %f\n\nPrevious State: %d\nAction Exec: %d\nReward Earned: %d\nCurrent(next) State: %d", agent->gen, agent->epsilon, agent->genDropoff, agent->colDropoff, agent->discountFactor, agent->alpha, STATE, ACTION, REWARD, FUTURESTATE);
        agent->overviewT.setString(temp);
        agent->overviewT.setPosition(window->getSize().x - agent->overviewT.getGlobalBounds().width, 0);

        agent->updateVisuall(this->player, mode == State::AI_yesEnemy ? true : false);

        // ....................................................................
    }

    // slider
    if (this->energyBar->vary(-(std::stof(Input::doc->first_node("data")->first_node("gameData")->first_node("sliderDecay")->value()))))
    {
        return false;
    }
    // energy
    if (this->recharger->inRange(player))
    {
        this->energyBar->setValue(100);
        this->player->increaseScore((rand() % 10) + 25);
        delete (recharger);
        recharger = new Energy();
        if (mode != State::User)
        {
            agent->epsilon = agent->epsilon - agent->colDropoff >= 0 ? agent->epsilon - agent->colDropoff : 0;
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
        MOT *i = *m;

        if (i->wait)
        {
            i->waiting();
            ++m;
            continue;
        }

        if (i->advance(deltaTime))
        {
            // is out of bouns
            this->spawner->listMOT.erase(m++);
            continue;
        }

        if (i->inRange(this->player))
        {
            if (mode != State::User)
                agent->epsilon = agent->epsilon + 0.3 <= 1 ? agent->epsilon + 0.3 : 1;
            return false;
        }
        ++m;
    }

    if (moveDelay != nullptr)
        moveDelay->restart();

    return true;
}

void Game::draw(sf::RenderWindow *window)
{

    window->draw(this->player->sprite);
    window->draw(this->recharger->sprite);

    window->draw(this->fps);
    window->draw(this->score);

    for (MOT *m : this->spawner->listMOT)
    {

        window->draw(m->wait ? m->spriteE : m->spriteM);
    }

    window->draw(this->energyBar->sBorder);
    window->draw(this->energyBar->sFill);
}