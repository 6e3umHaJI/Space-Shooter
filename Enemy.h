#pragma once
#include "Entity.h"
#include "Constants.h"
#include "Math.h"

class Enemy : public Entity {
protected:
	std::vector<Bullet> bullets;
	Time timeBulletUpdate, timeRotation;
	float angle, angleBetweenPlayer, distanceFromPlayer;
	float deltaBulletUpdate, deltaRotation;
	bool anim;

public:
	Enemy(Texture* texture, RenderWindow* window) :Entity(texture, Vector2f(0,0), 0.5f, 1.0f, 50, ENEMY_SCORE) {
		distanceFromPlayer = 200;
		deltaBulletUpdate = 600;
		angle = 0;
		angleBetweenPlayer = 0;
		deltaRotation = 10;
		spawn(window);
		anim = true;
	}
	
	float getAngle() {
		return angle;
	}

	void update(Vector2f playerPos, float time, Time currTime) {
		angleBetweenPlayer = atan2(playerPos.y - shape.getPosition().y, playerPos.x - shape.getPosition().x);
		float rotation = 0.005f;
		float delta = (currTime.asMilliseconds() - timeRotation.asMilliseconds() + TIME_CYCLE) % TIME_CYCLE;
		if (delta > deltaRotation && abs(angleBetweenPlayer - angle) > rotation)
		{
			timeRotation = currTime;
			angle = rotationAngle(angle, angleBetweenPlayer, delta*rotation);
		shape.setRotation(angle * 180 / PI - 90);
		rotateBody();
		}
		if (sqrt(pow(playerPos.x - shape.getPosition().x, 2) + pow(playerPos.y - shape.getPosition().y, 2)) > distanceFromPlayer)
		{
			shape.move(std::cos(angle) * speed * time, std::sin(angle) * speed * time);
			moveBody();
		}
	}

	void moveOutOfBounds(float time, Sprite *window) {
			shape.move(std::cos(angle) * speed * time, std::sin(angle) * speed * time);
			moveBody();
			if (shape.getGlobalBounds().intersects(window->getGlobalBounds()) == false)
			{
				dead = true;
				anim = false;
			}
	}
	
	bool isAnim() {
		return anim;
	}

	void addBullets(Bullet bullet, Time currTime) {
		if (((currTime - timeBulletUpdate).asMilliseconds() + TIME_CYCLE) % TIME_CYCLE > deltaBulletUpdate && abs(angle - angleBetweenPlayer) * 180 / PI < 30)
		{
			timeBulletUpdate = currTime;
			bullets.push_back(bullet);
		}
	}

	void updateBullets(Sprite window, std::list<Entity*> entities, float time) {
		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i].update(&window, entities, time);
			if (bullets[i].isDead())
			{
				bullets.erase(bullets.begin() + i);
			}
		}
	}

	void drawBullets(RenderWindow* window) {
		for (int i = 0; i < bullets.size(); i++) 
			window->draw(bullets[i].getShape());		
	}

	void spawn(RenderWindow* window) {
		srand(time(0));
		Vector2i windowSize(window->getSize().x, window->getSize().y);
	//	windowSize = Vector2i(window->mapPixelToCoords(windowSize).x, window->mapPixelToCoords(windowSize).y);
		int side = rand() % 4;
		switch (side) {
		case 0:
			shape.setPosition(Vector2f(-shape.getGlobalBounds().width, rand() % windowSize.y));
			break;
		case 1:
			shape.setPosition(Vector2f(windowSize.x + shape.getGlobalBounds().width, rand() % windowSize.y)) ;
			break;
		case 2:
			shape.setPosition(Vector2f(rand() % windowSize.x, -shape.getGlobalBounds().height)) ;
			break;
		case 3:
			shape.setPosition(Vector2f(rand() % windowSize.x, windowSize.y + shape.getGlobalBounds().height));
			break;
		}
	}
};
