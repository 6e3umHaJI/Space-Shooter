#pragma once
#include "Bullet.h"
#include "Entity.h"
#include "Constants.h"
#include "Math.h"

class Player : public Entity{
protected:
	std::vector<Bullet> bullets;
	Time timeBulletUpdate, timeRotation, timeRecover;
	float angle;
	float deltaBulletUpdate, deltaRotation, deltaRecover;
	SoundBuffer shootBuffer;
	Sound shoot;

public:
	Player() :Entity() {
	}

	Player(Texture* texture, Vector2i window) :Entity(texture, Vector2f(500,500), 1.5f, 0.1f, 100, 0) {
		angle = 0;
		deltaBulletUpdate = 250;
		deltaRotation = 10;
		deltaRecover = 500;
		spawn(window);
		body.setPosition(shape.getPosition());
		shootBuffer.loadFromFile("sounds/shoot.ogg");
		shoot.setBuffer(shootBuffer);
	}
	void setAngle(float angle) {
		this->angle = angle*PI/180;
		shape.setRotation(angle - 90);
	}
	float getAngle() {
		return angle+PI;
	}

	void move(Vector2f newPos, std::vector<RectangleShape> borders) {
		bool isIntersect = false;
		int i = 0;
		shape.setPosition(Vector2f(shape.getPosition().x + newPos.x, Object::getPosition().y + newPos.y));
		while (i < borders.size() && isIntersect == false) {
			if (shape.getGlobalBounds().intersects(borders[i].getGlobalBounds()))
				isIntersect = true;
			i++;
		}
		if (isIntersect)
			shape.setPosition(Vector2f(shape.getPosition().x - newPos.x, Object::getPosition().y - newPos.y));
		moveBody();
	}

	void rotateTowardsCursor(Time currTime, Vector2i mousePos, std::vector<RectangleShape> borders) {
		float delta = (currTime.asMilliseconds() - timeRotation.asMilliseconds() + TIME_CYCLE) % TIME_CYCLE;
		if (delta > deltaRotation)
		{
			timeRotation = currTime;
			bool isIntersect = false;
			int i = 0;
			float dX = shape.getPosition().x - mousePos.x;
			float dY = shape.getPosition().y - mousePos.y;
			float fullAngle = (std::atan2(dY, dX));
			float rotationPrev = shape.getRotation();
			float anglePrev = angle;
			angle = rotationAngle(angle, fullAngle, 0.01f*delta);
			shape.setRotation(angle * 180 / PI - 90);
			while (i < borders.size() && isIntersect == false) {
				if (shape.getGlobalBounds().intersects(borders[i].getGlobalBounds()))
					isIntersect = true;
				i++;
			}
			if (isIntersect)
			{
				shape.setRotation(rotationPrev);
				angle = anglePrev;
			}
			rotateBody();
		}
	}

	void addBullets(Bullet bullet, Time currTime) {
		if (((currTime - timeBulletUpdate).asMilliseconds() + TIME_CYCLE)% TIME_CYCLE > deltaBulletUpdate)
		{
			timeBulletUpdate = currTime;
			bullets.push_back(bullet);
			shoot.setBuffer(shootBuffer);
			shoot.play();
		}
	}


	void increaseScore(int delta) {
		score += delta;
	}

	void updateBullets(Sprite window, std::list<Entity*> entities, float time) {
		for (int i = 0; i < bullets.size(); i++)
		{
			increaseScore(bullets[i].update(&window, entities, time));
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

	void recover(Time currTime) {
		if (((currTime - timeRecover).asMilliseconds() + TIME_CYCLE) % TIME_CYCLE > deltaRecover) {
			timeRecover = currTime;
			healthBar.recoverHealth(1);
		}
	}
	
	void spawn(Vector2i window) {
		shape.setPosition(Vector2f(window.x * 0.5, window.y * 0.5));
	}
};