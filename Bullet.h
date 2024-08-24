#pragma once
#include "Object.h"
#include "Math.h"
#include "Entity.h"
#include "Player.h"
#include <list>
#include <iterator>
#include "Constants.h"
using namespace sf;

class Bullet : public Object {
protected:
	float angle;
	bool dead;
	Time prevTime;
	int damage;
	float speed;

public:
	Bullet(Texture* texture, Vector2f pos, Vector2i posMouse, int damage, float speed):Object(texture, pos, 0.5f) {
		angle = atan2(posMouse.y - pos.y, posMouse.x - pos.x);
		shape.setRotation(angle * 180 / PI - 90);
		dead = false;
		this->damage = damage;
		this->speed = speed;
	}

	Bullet(Texture* texture, Vector2f pos, float angle, int damage, float speed) :Object(texture, pos, 0.5f) {
		this->angle = angle;
		shape.setRotation(angle * 180 / PI - 90);
		dead = false;
		this->damage = damage;
		this->speed = speed;
	}
	
	int getDamage() {
		return damage;
	}

	void setDamage(int damage) {
		this->damage = damage;
	}

	bool isDead() {
		return dead;
	}

	void setPos(Vector2f playerPos) {
		shape.setPosition(playerPos);
	}

	char update(Sprite* window, std::list<Entity*> entities, float time) {
		char score = 0;
		std::list<Entity*>::iterator it;
		shape.move(std::cos(angle) * speed * time, std::sin(angle) * speed * time);
		for (it = entities.begin(); it != entities.end(); it++)
			if (shape.getGlobalBounds().intersects((*it)->getBody().getGlobalBounds()))
			{
					dead = true;
					(*it)->reduceHealth(damage);
					if ((*it)->isDead()) {
						score = (*it)->getScore();
					}
			}
		if (shape.getGlobalBounds().intersects(window->getGlobalBounds()) == false)
			dead = true;
		return score;
	}
};
