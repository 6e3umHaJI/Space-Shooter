#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "MainMenu.h"
#include <SFML/Audio.hpp>
#include <sstream>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Border.h"
#include "Animations.h"
#include "Entity.h"
#include <list>
#include <iterator>
#include "Constants.h"
#include "NicknameForm.h"
#include "Table.h"
#include "Menu.h"

using namespace sf;

class Game {
protected:
    enum State {
        mainMenu,
        gameMenu
    };
    State state;
    RenderWindow* window;

    Texture bgTex;
    Sprite bg;
    Texture bgTextTex;
    Sprite bgText;
    Texture rulesTex;
    Object rules;
    MainMenu menuMain;
    Menu menu;

    Texture playerTex, enemyTex;
    Texture bulletTex, bulletEnemyTex;
    Texture asteroidTex1, asteroidTex2, asteroidTex3;
    Texture explosionTex1, explosionTex2, explosionTex3;
    Texture smokeTex;

    Player player;
    RectangleShape rect;
    Border border;
    std::list<Enemy*> enemy;
    std::list<Enemy*>::iterator itEnemy;
    std::list<Asteroid*> asteroid;
    std::list<Asteroid*>::iterator itAsteroid;

    //Text init
    Font font, font2;
    Text playerScore, gameOver;

    //Time init
    Clock clock, speedClock;
    Time currTime, prevTimeEnemyPush, prevTimeAsteroidPush;

    std::list<Entity*> entities;
    std::list<Entity*>::iterator itEntities;
    SoundBuffer shootBuffer;
    Sound shoot;
    SoundBuffer explosionBuffer1;
    Sound explosion1;
    SoundBuffer explosionBuffer2;
    Sound explosion2;
    SoundBuffer explosionBuffer3;
    Sound explosion3;
    SoundBuffer damageBuffer;
    Sound damage;
    Music music;

    Animations animations;
    float time;

    bool isGameEnded;

    NicknameForm nicknameForm;
    Record record;
    Table table;

public:
    Game(RenderWindow* window) {
        state = mainMenu;
        this->window = window;
        bgTex.loadFromFile("textures/bg.png");
        bg.setTexture(bgTex);
        bgTextTex.loadFromFile("textures/bgText.png");
        bgText.setTexture(bgTextTex);
        rulesTex.loadFromFile("textures/rules.png");
        rules = Object(&rulesTex);
        Vector2f position = Vector2f(window->getSize().x * 0.5, window->getSize().y * 0.4);
        rules.setPosition(position);

        playerTex.loadFromFile("textures/ship.png");
        bulletTex.loadFromFile("textures/bullet.png");
        bulletEnemyTex.loadFromFile("textures/bullet2.png");
        enemyTex.loadFromFile("textures/enemy.png");
        asteroidTex1.loadFromFile("textures/asteroid1.png");
        asteroidTex2.loadFromFile("textures/asteroid2.png");
        asteroidTex3.loadFromFile("textures/asteroid3.png");
        explosionTex1.loadFromFile("textures/explos.png");
        explosionTex2.loadFromFile("textures/explos2.png");
        explosionTex3.loadFromFile("textures/explos3.png");
        smokeTex.loadFromFile("textures/smoke.png");

        player = Player(&playerTex, Vector2i(window->getSize()));
        rect.setFillColor(Color::Black);
        border = Border(window, &rect);
        font.loadFromFile("fonts/SignikaNegative.ttf");
        playerScore = Text("", font, 50);
        playerScore.setFillColor(Color::White);
        playerScore.setPosition(40, 20);
        font2.loadFromFile("fonts/Roboto.ttf");
        gameOver = Text("Game over. Press enter to go back", font2, 70);
        gameOver.setFillColor(Color::White);
        gameOver.setOrigin(Vector2f(gameOver.getLocalBounds().width*0.5, gameOver.getLocalBounds().height * 0.5));
        gameOver.setPosition(window->getSize().x * 0.5, window->getSize().y * 0.5);
        shootBuffer.loadFromFile("sounds/shoot.ogg");
        shoot = Sound(shootBuffer);
        explosionBuffer1.loadFromFile("sounds/explosion.ogg");
        explosion1 = Sound(explosionBuffer1);
        explosionBuffer2.loadFromFile("sounds/explosion2.ogg");
        explosion2 = Sound(explosionBuffer2);
        explosionBuffer3.loadFromFile("sounds/explosion3.ogg");
        explosion3 = Sound(explosionBuffer3);
        damageBuffer.loadFromFile("sounds/damage.ogg");
        damage = Sound(damageBuffer);
        music.openFromFile("sounds/bgMusic.ogg");
        music.setVolume(30);
        music.play();
        music.setLoop(true);

        time = 0;
        isGameEnded = false;

        nicknameForm = NicknameForm();
        table = Table("data.txt");
    }

    void spawnEnemy() {
        //Спавн врагов
        if (((currTime - prevTimeEnemyPush).asMilliseconds() + TIME_CYCLE) % TIME_CYCLE > 2000 && enemy.size() < ENEMY_AMOUNT_MAX) {
            prevTimeEnemyPush = currTime;
            enemy.push_back(new Enemy(&enemyTex, window));
        }
    }

    void spawnAsteroid() {
        //Спавн астероидов
        if (((currTime - prevTimeAsteroidPush).asMilliseconds() + TIME_CYCLE) % TIME_CYCLE > 1000 && asteroid.size() < ASTEROID_AMOUNT_MAX) {
            prevTimeAsteroidPush = currTime;
            switch (rand() % 3) {
            case 0:
                asteroid.push_back(new Asteroid(&asteroidTex1, window, Vector2f(1.4, 1.2)));
                break;
            case 1:
                asteroid.push_back(new Asteroid(&asteroidTex2, window, Vector2f(1.4, 1.2)));
                break;
            case 2:
                asteroid.push_back(new Asteroid(&asteroidTex3, window, Vector2f(1.4, 1.2)));
                break;
            }
        }
    }

    void updateEnemies() {
        // Обновляем врагов(изменение позиции, добавление пули с выстрелом, обновление пуль врагов)
        itEnemy = enemy.begin();
        while (itEnemy != enemy.end()) 
        {
            if ((*itEnemy)->isDead() == false) {
                (*itEnemy)->updateBullets(bg, entities, time);
                if (player.isDead())
                {
                    (*itEnemy)->moveOutOfBounds(time, &bg);
                }
                else
                {
                    (*itEnemy)->update(player.getPosition(), time, currTime);
                    (*itEnemy)->addBullets(Bullet(&bulletEnemyTex, (*itEnemy)->getPosition(), (*itEnemy)->getAngle(), 10, 2), currTime);
                }
            }
            itEnemy++;
        }
    }

    void updateAsteroid() {
        // Обновляем астероиды
        itAsteroid = asteroid.begin();
        while (itAsteroid != asteroid.end()) {
            if ((*itAsteroid)->isDead() == false)
                (*itAsteroid)->update(&bg, entities, currTime, time);
            itAsteroid++;
        }
    }

    void checkEnemiesState() {
        itEnemy = enemy.begin();
        while (itEnemy != enemy.end()) {
            if ((*itEnemy)->isDead()) {
                if ((*itEnemy)->isAnim()) {
                    animations.addAnim(Animator(&explosionTex1, (*itEnemy)->getPosition(), 8, 6, Vector2i(256, 248), 1, false, 0.5f, false));
                    explosion1.play();
                }
                Enemy* pt = (*itEnemy);
                itEnemy = enemy.erase(itEnemy);
                delete pt;
            }
            else
                itEnemy++;
        }
    }

    void checkAsteroidsState() {
        itAsteroid = asteroid.begin();
        while (itAsteroid != asteroid.end()) {
            if ((*itAsteroid)->isDead()) {
                if ((*itAsteroid)->isAnim())
                {
                    explosion2.play();
                    animations.addAnim(Animator(&smokeTex, (*itAsteroid)->getPosition(), 6, 5, Vector2i(256, 255.8), 1, false, 0.5f, true));
                }
                Asteroid* pt = (*itAsteroid);
                itAsteroid = asteroid.erase(itAsteroid);
                delete pt;
            }
            else
                itAsteroid++;
        }
    }

    void updatePlayer() {
        // Поворачиваем игрока в сторону курсора
        player.rotateTowardsCursor(currTime, Mouse::getPosition(*window), border.getBorders());

        // Обновляем пули игрока
        player.updateBullets(bg, entities, time);
        player.recover(currTime);

        // Взаимодействуем с игроком в зависимости от нажатых клавиш
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
        {
            player.move(Vector2f(0, -player.getSpeed() * time), border.getBorders());
        }
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
        {
            player.move(Vector2f(-player.getSpeed() * time, 0), border.getBorders());
        }
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
        {
            player.move(Vector2f(0, player.getSpeed() * time), border.getBorders());
        }
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
        {
            player.move(Vector2f(player.getSpeed() * time, 0), border.getBorders());
        }
        if (Mouse::isButtonPressed(Mouse::Left) || Keyboard::isKeyPressed(Keyboard::Space))
        {
            player.addBullets(Bullet(&bulletTex, player.getPosition(), player.getAngle(), 10, 3), currTime);
        }
    }

    void updateTime() {
        if (clock.getElapsedTime().asMilliseconds() > TIME_CYCLE) {
            clock.restart();
        }   
        
        currTime = clock.getElapsedTime();
        time = speedClock.getElapsedTime().asMicroseconds();
        speedClock.restart();
        time = time / 1600; //скорость игры
    }

    void clearAsteroidList() {
        while (!asteroid.empty())
        {
            Asteroid* pt = asteroid.front();
            asteroid.pop_front();
            delete pt;
        }
    }

    void clearEnemyList() {
        while (!enemy.empty())
        {
            Enemy* pt = enemy.front();
            enemy.pop_front();
            delete pt;
        }
    }
    
    void resetTime() {
        time = 0;
        currTime = milliseconds(0);
        prevTimeEnemyPush = milliseconds(0);
        prevTimeAsteroidPush = milliseconds(0);
        resetClock();
    }

    void resetClock() {
        clock.restart();
        speedClock.restart();
    }

    void resetGameMenu() {
        player = Player(&playerTex, Vector2i(window->getSize()));
        isGameEnded = false;
        clearAsteroidList();
        clearEnemyList();
        resetTime();
        animations.clear();
        entities.clear();
        menu.reset();
        menu.close();
    }

    void resetBackground() {
        while (!asteroid.empty())
        {
            Asteroid* pt = asteroid.front();
            asteroid.pop_front();
            delete pt;
        }
    }

    void resetMainMenu() {
        menuMain.reset();
        music.play();
        table.reset();
    }

    void updateGameMenu() {
        updateTime();
        if (isGameEnded == false && player.isDead()) {
            animations.addAnim(Animator(&explosionTex3, player.getPosition(), 8, 8, Vector2i(256, 256), 2, false, 1.0f, false));
            explosion3.play();
            isGameEnded = true;
        }
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                window->close();
            if (player.isDead() && event.type == Event::KeyReleased && event.key.code == Keyboard::Enter)
            {
                nicknameForm.setScore(player.getScore());
                resetGameMenu();
                resetMainMenu();
                state = mainMenu;
                nicknameForm.activate();
                return void();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && !player.isDead())
            {
                menu.open();
                return void();
            }
        }

        animations.Update(currTime);

        // Очищаем entities list
        entities.clear();

        // Помещаем в entities list игрока
        entities.push_front(&player);

        // Помещаем в entities list астероиды
        for (itAsteroid = asteroid.begin(); itAsteroid != asteroid.end(); itAsteroid++)
            entities.push_back(*itAsteroid);

            updateEnemies();

        // Помещаем в entities list врагов
        for (itEnemy = enemy.begin(); itEnemy != enemy.end(); itEnemy++)
            entities.push_back(*itEnemy);

        updateAsteroid();

        entities.pop_front();
        if (player.isDead() == false) {
            updatePlayer();
            spawnEnemy();
            spawnAsteroid();
        }

        checkAsteroidsState();
        checkEnemiesState();
    }

    void updateBackground() {
        updateTime();
        spawnAsteroid();
        updateAsteroid();
        checkAsteroidsState();
    }

    void update() {
        switch (state) {
        case mainMenu:
        {
            updateBackground();
            if (nicknameForm.isActivate()) {
                nicknameForm.update(window, &record);
                if (!nicknameForm.isActivate())
                    table.addRecord(record);
                return void();
            }
            menuMain.update(window);
            switch (menuMain.getActiveButton()) {
            case 0:
                resetBackground();
                state = gameMenu;
                break;
            case 1:
                table.update(window);
                break;
            case 2:
                rules.drawObject(window);
                break;
            case 3:
                table.loadFile();
                window->close();
                break;
            }
            break;
        }
        case gameMenu:
            if (menu.isActivated()) {
                menu.update(window);
                switch (menu.getActiveButton()) {
                case 0:
                    menu.close();
                    menu.reset();
                    resetTime();
                    break;
                case 1:
                    resetGameMenu();
                    music.play();
                    break;
                case 2:
                    resetGameMenu();
                    resetMainMenu();
                    state = mainMenu;
                    break;
                case 3:
                    table.loadFile();
                    window->close();
                    break;
                }
            }
            else
                updateGameMenu();
            break;
        }
    }

    void drawBackground() {
        for (itAsteroid = asteroid.begin(); itAsteroid != asteroid.end(); itAsteroid++)
            (*itAsteroid)->drawObject(window);
    }

	void draw() {
        window->clear();
        window->draw(bg);
        switch (state) {
        case mainMenu:
            drawBackground();
            menuMain.draw(window);
            switch (menuMain.getActiveButton()) {
            case -1:
                if (nicknameForm.isActivate())
                    nicknameForm.draw(window);
                else
                window->draw(bgText);
                break;
            case 1:
                table.draw(window);
                break;
            case 2:
                rules.drawObject(window);
                break;
            }
            break;
        case gameMenu:
            //Отрисовка врагов
            for (itEnemy = enemy.begin(); itEnemy != enemy.end(); itEnemy++) {
                (*itEnemy)->drawBullets(window);
                (*itEnemy)->drawEntity(window);
            }
            //Отрисовка анимации
            for (int i = 0; i < animations.getAnimations().size(); i++)
                window->draw(animations.getAnimations()[i].getShape());

            //Отрисовка игрока
            if (player.isDead())
                window->draw(gameOver);
            else {
                player.drawBullets(window);
                player.drawEntity(window);
            }
            //Отрисовка астероидов
            for (itAsteroid = asteroid.begin(); itAsteroid != asteroid.end(); itAsteroid++)
                (*itAsteroid)->drawObject(window);
            //Отрисовка счета игрока
            std::ostringstream playerScoreString;
            playerScoreString << player.getScore();
            playerScore.setString("Score: " + playerScoreString.str());
            window->draw(playerScore);
            if (menu.isActivated())
                menu.draw(window);
            break;
        }
        window->display();
	}
};
