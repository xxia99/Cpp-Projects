/*
Author: <Xin Xia>
Class: ECE6122
Last Date Modified: <10/26/2020>
Description: To create a derivation of ¡°Angry Birds¡± called ¡°Buzzy¡¯s Revenge¡± using the SFML API
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "Utilizations.h"
using namespace std;
using namespace sf;

int reX = 145;
int reY = 500; 
int reAngle = -50;
int firstPos = 1600;
int secondPos = 1800;
int lifeNum = 5;
int totalRewards = 99;
float pStorage = 0;
bool startGame = false;
bool keepPlaying = false;
bool firstExist = true;
bool secondExist = true;
vector<Icon*> evilMas, wlCrea, firstColumn, secondColumn;

random_device device;
default_random_engine engine(device());
uniform_int_distribution<int> ind_distribution(0, 9);
uniform_int_distribution<int> dy_distribution(0, 2);
uniform_int_distribution<int> y_distribution(150, 750);
uniform_int_distribution<int> speed_distribution(150, 500);

Vector2f positionInitializer(const int index)
{
    Vector2f ranPos;
    switch (index)
    {
    case 0:
        ranPos.x = firstPos;
        ranPos.y = 300;
        break;
    case 1:
        ranPos.x = firstPos;
        ranPos.y = 450;
        break;
    case 2:
        ranPos.x = firstPos;
        ranPos.y = 600;
        break;
    case 3:
        ranPos.x = firstPos;
        ranPos.y = 750;
        break;
    case 4:
        ranPos.x = firstPos;
        ranPos.y = 900;
        break;
    case 5:
        ranPos.x = secondPos;
        ranPos.y = 300;
        break;
    case 6:
        ranPos.x = secondPos;
        ranPos.y = 450;
        break;
    case 7:
        ranPos.x = secondPos;
        ranPos.y = 600;
        break;
    case 8:
        ranPos.x = secondPos;
        ranPos.y = 750;
        break;
    case 9:
        ranPos.x = secondPos;
        ranPos.y = 900;
        break;
    }
    return ranPos;
}
void patchResizer()
{
    vector<int> imgQueue;
    for (int i = 0; i < 10; ++i)
    {
        imgQueue.push_back(i);
    }

    int n = imgQueue.contentRange();
    do
    {
        for (int i = 0; i < n; i++)
        {
            int index = i + ind_distribution(engine) % (n - i);
            swap(imgQueue[index], imgQueue[i]);
        }
    } while (!(imgQueue[0] < 5 && imgQueue[1] >= 5));
    for (int i = 0; i < evilMas.contentRange(); i++)
    {
        (*evilMas[i]).setPosition(positionInitializer(imgQueue[i]));
        (*evilMas[i]).setRotation(0.f);
        (*evilMas[i]).theInd = imgQueue[i];
        (*evilMas[i]).beHit = false;
    }
    for (int j = 0; j < wlCrea.contentRange(); j++)
    {
        (*wlCrea[j]).setPosition(positionInitializer(imgQueue[j + 2]));
        (*wlCrea[j]).setRotation(0.f);
        (*wlCrea[j]).theInd = imgQueue[j + 2];
        (*wlCrea[j]).beHit = false;
    }
    firstColumn.clear();
    secondColumn.clear();
    for (int i = 0; i < wlCrea.contentRange(); ++i)
    {
        if (i != 4)
        {
            if ((*wlCrea[i]).theInd < 5)
                firstColumn.push_back(wlCrea[i]);
            else if ((*wlCrea[i]).theInd >= 5)
                secondColumn.push_back(wlCrea[i]);
        }
    }
}
void totalInitializer()
{
    secondExist = true;
    firstExist = true;
    patchResizer();
    lifeNum = 5;
    totalRewards = 0;
    pStorage = 0.;
}
void iconAligner(Icon& rmIcon)
{
    if (rmIcon.theInd < 5)
    {
        for (auto w : firstColumn)
        {
            if ((*w).theInd < rmIcon.theInd)
            {
                (*w).theInd++;
                (*w).setPosition((*w).getPosition().x, (*w).getPosition().y + 150);
            }
        }
    }
    else
    {
        for (auto w : secondColumn)
        {
            if ((*w).theInd < rmIcon.theInd)
            {
                (*w).theInd++;
                (*w).setPosition((*w).getPosition().x, (*w).getPosition().y + 150);
            }
        }
    }
    for (auto m : evilMas)
    {
        if ((*m).theInd < rmIcon.theInd)
        {
            (*m).theInd++;
            (*m).setPosition((*m).getPosition().x, (*m).getPosition().y + 150);
        }
    }
}
int main()
{
    RenderWindow window(VideoMode(960, 540), "Buzzy's Revenge");
    window.setView(View(FloatRect(0, 0, 1920, 1080)));
    // Define the background
    Icon background("./graphics/background.png", 0, 0, 1, 1);
    background.setOrigin(0, 0);
    vector<Icon> lifeBar;
    Icon life1("./graphics/buzzy_life.png", 170, 50, 0.5, 0.5);
    lifeBar.push_back(life1);
    Icon life2("./graphics/buzzy_life.png", 230, 50, 0.5, 0.5);
    lifeBar.push_back(life2);
    Icon life3("./graphics/buzzy_life.png", 290, 50, 0.5, 0.5);
    lifeBar.push_back(life3);
    Icon life4("./graphics/buzzy_life.png", 350, 50, 0.5, 0.5);
    lifeBar.push_back(life4);
    Icon life5("./graphics/buzzy_life.png", 410, 50, 0.5, 0.5);
    lifeBar.push_back(life5);
    // Define the powerbar
    PowerBar powerBar(500, 50, 'r', 150, 925);
    // Define the columns
    Icon sheep("./graphics/sheep.png", firstPos, 300, 1.5, 1.5);
    wlCrea.push_back(&sheep);
    firstColumn.push_back(&sheep);
    Icon bunny("./graphics/bunny.png", firstPos, 450, 1.6, 1.6);
    wlCrea.push_back(&bunny);
    firstColumn.push_back(&bunny);
    Icon dog("./graphics/dog.png", firstPos, 600, 1.5, 1.5);
    wlCrea.push_back(&dog);
    firstColumn.push_back(&dog);
    Icon Tiger("./graphics/tiger.png", firstPos, 750, 0.17, 0.17);
    Tiger.setOrigin(500, 500);
    evilMas.push_back(&Tiger);
    Icon mouse("./graphics/mouse.png", firstPos, 900, 1.4, 1.4);
    wlCrea.push_back(&mouse);
    firstColumn.push_back(&mouse);
    Icon unicorn("./graphics/angry_unicorn.png", secondPos, 300, 1.5f, 1.5f);
    wlCrea.push_back(&unicorn);
    Icon frog("./graphics/frog.png", secondPos, 450, 1.4f, 1.4f);
    wlCrea.push_back(&frog);
    secondColumn.push_back(&frog);
    Icon Bulldog("./graphics/Bulldog.png", secondPos, 590, 1.8f, 1.8f);
    evilMas.push_back(&Bulldog);
    Icon pig("./graphics/pig.png", secondPos, 750, 1.4f, 1.4f);
    wlCrea.push_back(&pig);
    secondColumn.push_back(&pig);
    Icon chicken("./graphics/chicken.png", secondPos, 900, 1.8f, 1.6f);
    wlCrea.push_back(&chicken);
    secondColumn.push_back(&chicken);
    // Define text contents
    TextStyle titleLine("./fonts/KOMIKAP_.ttf", "Buzzy's Revenge", 80, 'r', 960, 200);
    TextStyle restartLine("./fonts/KOMIKAP_.ttf", "Press Enter to Restart Game", 50, 'w', 960, 400);
    TextStyle exitLine("./fonts/KOMIKAP_.ttf", "Press Esc to Exit", 50, 'w', 960, 460);
    TextStyle powerLine("./fonts/KOMIKAP_.ttf", "Press Space to Powerup", 50, 'w', 960, 520);
    TextStyle aimLine("./fonts/KOMIKAP_.ttf", "Press up/down arrow to Aim", 50, 'w', 960, 580);
    TextStyle authorLine("./fonts/KOMIKAP_.ttf", "Created by\n Xin Xia", 25, 'w', 960, 750);
    TextStyle powerText("./fonts/KOMIKAP_.ttf", "Power", 30, 'w', 80, 950);
    TextStyle lifeText("./fonts/KOMIKAP_.ttf", "Lives", 30, 'w', 80, 50);
    TextStyle scoreText("./fonts/KOMIKAP_.ttf", "Score: 99", 30, 'w', 1750, 50);
    TextStyle overText("./fonts/KOMIKAP_.ttf", "Game Over", 80, 'r', 960, 200);
    // Define the sounds used for hitting, winning and losing
    BGM hit("./sound/chop.wav");
    BGM win("./sound/out_of_time.wav");
    BGM gameOver("./sound/death.wav");
    // The Buzzy
    Buzzy Revenger("./graphics/smallbuzzy.png", 250, 500, 1.4, 1.4);
    Revenger.rotate(reAngle);
    Revenger.setOrigin(200, 40);
    Track flyCurve;
    // The Insect
    Icon Bee("./graphics/Bee.png", 1600, 200, 1.5, 1.5);
    Bee.setScale(-1.5, 1.5);
    Bee.setOrigin(25, 25);
    // Create the window
    while (window.isOpen())
    {
        Event event;
        Clock clock;
        // Conert operations to signals in the game window
        while (window.pollEvent(event))
        {
            // Enter the game
            if (event.key.code == Keyboard::Enter)
            {
                totalInitializer();
                startGame = true; // Start-game Flag
                flyCurve.reset();
                Bee.beHit = false;
                powerBar.setSize(pStorage);
                scoreText.computeScore(totalRewards);
            }
            // Operations in a game
            if (startGame && Keyboard::isKeyPressed(Keyboard::Up))
            {
                Revenger.asCend();
            }
            if (startGame && Keyboard::isKeyPressed(Keyboard::Down))
            {
                Revenger.desCend();
            }
            if (startGame && event.key.code == Keyboard::Space && pStorage < powerBar.barLength)
            {
                pStorage += 10;
                powerBar.setSize(pStorage);
                Revenger.howFast = 4 * pStorage + 400;
            }
            if (startGame && event.type == Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            {
                clock.restart();
                flyCurve.reset();
                flyCurve.normLizer(Revenger.posX, Revenger.posY, Revenger.howFast, Revenger.getRotation());
                Revenger.canFly = true;
                keepPlaying = true;
                powerBar.setSize(0);
            }           
            // Exit the game and close the window
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }
        // Rules in the game
        if (startGame == false) // The initial page
        {
            background.draw(window);
            titleLine.draw(window);
            restartLine.draw(window);
            exitLine.draw(window);
            powerLine.draw(window);
            aimLine.draw(window);
            authorLine.draw(window);
            Revenger.draw(window);
            Bee.draw(window);
            powerBar.barBuilder(window);
            lifeText.draw(window);
            for (int i = 0; i < lifeNum; ++i)
                lifeBar[i].draw(window);
            powerText.draw(window);
            scoreText.draw(window);
            titleLine.draw(window);
        }
        else // Enter the game
        {
            Time startTime = clock.restart();
            window.clear();
            background.draw(window);
            Revenger.draw(window);
            if (!Bee.beHit)
                Bee.draw(window);
            lifeText.draw(window);
            for (int i = 0; i < lifeNum; ++i)
                lifeBar[i].draw(window);
            powerText.draw(window);
            powerBar.barBuilder(window);
            scoreText.draw(window);

            // Set the status of the bee
            if (Bee.canFly)
            {
                int route = Bee.getPosition().y;
                int dy = dy_distribution(engine);
                switch (dy)
                {
                case 0:
                    break;
                case 1:
                    route += 1;
                    break;
                case 2:
                    route -= 1;
                    break;
                }
                float flyTime = startTime.asSeconds();
                Bee.setPosition(Bee.getPosition().x - flyTime * Bee.howFast, route);

                if (0 <= Bee.getPosition().x <= 1920 && 0 <= Bee.getPosition().y <= 1080)
                    Bee.canFly = Bee.canFly;
                else
                {
                    Bee.canFly = false;
                }
            }
            else
            {
                Bee.setPosition(1920, y_distribution(engine));
                Bee.howFast = speed_distribution(engine);
                Bee.canFly = true;
            }

            // Set the rules for Revenger in a round of the game    
            if (keepPlaying)
            {
                float buzzyTime = startTime.asSeconds();
                flyCurve.t += buzzyTime;
                //   Launch the Revenger
                Revenger.setPosition(flyCurve.getXPosition(), flyCurve.getYPosition());
                Revenger.setRotation(flyCurve.calDirect());
                // if the Revenger hit tiger
                if (!Tiger.beHit && Revenger.hitItems(Tiger.getPosition()))
                {
                    hit.play();
                    totalRewards += 25;
                    scoreText.computeScore(totalRewards);
                    Tiger.beHit = true;
                    if (Tiger.theInd < 5)
                        firstExist = false;
                    else
                        secondExist = false;
                    firstExist = false;
                    keepPlaying = false;
                    pStorage = 0;
                    Revenger.reset(reX, reY, reAngle);
                }
                // if the Revenger hit tigerhit Bulldog
                else if (!Bulldog.beHit && Revenger.hitItems(Bulldog.getPosition()))
                {
                    hit.play();
                    totalRewards += 25;
                    scoreText.computeScore(totalRewards);
                    Bulldog.beHit = true;
                    if (Bulldog.theInd < 5)
                        firstExist = false;
                    else
                        secondExist = false;
                    keepPlaying = false;
                    pStorage = 0;
                    Revenger.reset(reX, reY, reAngle);
                }
                //  if the Revenger hit the bee
                else if (!Bee.beHit && Revenger.intersectSmall(Bee.getPosition()))
                {
                    totalRewards += 75;
                    hit.play();
                    scoreText.computeScore(totalRewards);
                    Bee.beHit = true;
                    keepPlaying = false;
                    pStorage = 0;
                    Revenger.reset(reX, reY, reAngle);
                }
                // if the Revenger hit the unicorn
                else if (!unicorn.beHit && Revenger.hitItems(unicorn.getPosition()))
                {
                    if (lifeNum < 5)
                    {
                        lifeNum += 1;
                    }
                    unicorn.beHit = true;
                    hit.play();
                    iconAligner(unicorn);
                    pStorage = 0;
                    keepPlaying = false;
                    Revenger.reset(reX, reY, reAngle);
                }
                //  if the Revenger hit woodland creatures
                else if ((Revenger.crash(firstColumn))|| (Revenger.crash(secondColumn)))
                {
                    if (lifeNum > 1)
                    {
                        lifeNum -= 1;
                        hit.play();
                        keepPlaying = false;
                        pStorage = 0;
                    }
                    else // Game Over
                    {
                        gameOver.play();
                        totalInitializer();
                        startGame = false;
                        Bee.beHit = false;
                        flyCurve.reset();
                        powerBar.setSize(pStorage);
                        scoreText.computeScore(totalRewards);
                    }      
                    Revenger.reset(reX, reY, reAngle);
                }
                //  fly beyond the window
                else if ((Revenger.getPosition().x > 1920 || Revenger.getPosition().x < 0
                    || Revenger.getPosition().y > 1080 || Revenger.getPosition().y < 0))
                {
                    if (lifeNum > 1)
                    {
                        lifeNum -= 1;
                        keepPlaying = false;
                        pStorage = 0;
                    }
                    else // Game Over
                    {
                        gameOver.play();
                        totalInitializer();
                        startGame = false;
                        Bee.beHit = false;
                        flyCurve.reset();
                        powerBar.setSize(pStorage);
                        scoreText.computeScore(totalRewards);
                    }
                    Revenger.reset(reX, reY, reAngle);                    
                }
            }  
        }
        // Definition for winning
        if (Tiger.beHit && Bulldog.beHit)
        {
            win.play();
            totalInitializer();
            startGame = true;
            Bee.beHit = false;
            flyCurve.reset();
            powerBar.setSize(pStorage);
            scoreText.computeScore(totalRewards);
        }
        if (firstExist)
        {
            for (int i = 0; i < firstColumn.contentRange(); ++i)
                (*firstColumn[i]).draw(window);
            if (unicorn.theInd < 5 && !unicorn.beHit)
                unicorn.draw(window);
            if (Bulldog.theInd < 5)
                Bulldog.draw(window);
            if (Tiger.theInd < 5)
                Tiger.draw(window);
        }
        if (secondExist)
        {
            for (int i = 0; i < secondColumn.contentRange(); ++i)
                (*secondColumn[i]).draw(window);
            if (unicorn.theInd >= 5 && !unicorn.beHit)
                unicorn.draw(window);
            if (Tiger.theInd >= 5)
                Tiger.draw(window);
            if (Bulldog.theInd >= 5)
                Bulldog.draw(window);
        }
        window.display();
    }
    return 0;
}