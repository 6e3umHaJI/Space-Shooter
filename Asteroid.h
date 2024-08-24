#pragma once
#include "Entity.h"

using namespace sf;

class Asteroid : public Entity {
protected:
	Time timeRotation;
	float deltaRotation, deltaAngle, angle;
	bool anim;

public:
	Asteroid(Texture* texture, RenderWindow* window, Vector2f scale) :Entity(texture, Vector2f(500,500), (rand()%5+8)*0.1f, 0.5f, rand() % 20 + 10, ASTEROID_SCORE) {
		deltaRotation = 10;
		deltaAngle = 1;
		spawn(window);
		setBodyScale(scale);
		anim = true;
	}
	
	bool isAnim() {
		return anim;
	}


	void rotate(Time currTime) {
		if (((currTime - timeRotation).asMilliseconds() + TIME_CYCLE) % TIME_CYCLE > deltaRotation) {
			timeRotation = currTime;
			shape.rotate(deltaAngle);
			rotateBody();
		}
	}

	void update(Sprite *window, std::list<Entity*> entities, Time currTime, float time) {
		if (dead) return;
			shape.move(std::cos(angle) * speed * time, std::sin(angle) * speed * time);
			moveBody();
		    rotate(currTime);
			if (shape.getGlobalBounds().intersects(window->getGlobalBounds()) == false)
			{
				dead = true;
				anim = false;
			}
			if (dead == false) {
				std::list<Entity*>::iterator it;
				it = entities.begin();
				while (it != entities.end() && dead == false) {
					if (body.getGlobalBounds().intersects((*it)->getBody().getGlobalBounds()) && (*it)->getPosition() != shape.getPosition())
					{
						dead = true;
						(*it)->reduceHealth(healthBar.getHealth()*2);
					}
					else it++;
				}
			}
	}

	void spawn(RenderWindow* window) {
		srand(time(0));
		int side = rand() % 4;
		Vector2i windowSize(window->getSize().x, window->getSize().y);
	//	windowSize = Vector2i(window->mapPixelToCoords(windowSize).x, window->mapPixelToCoords(windowSize).y);
		switch(side){
		case 0:
			shape.setPosition(Vector2f(-shape.getGlobalBounds().width*0.4, rand() % windowSize.y));
			angle = PI*1.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5 * PI - PI * 1.5)));
			break;
		case 1:
			shape.setPosition(Vector2f(windowSize.x + shape.getGlobalBounds().width * 0.4, rand() % windowSize.y));
			angle = PI * 0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.5 * PI - PI * 0.5)));
			break;
		case 2:
			shape.setPosition(Vector2f(rand() % windowSize.x, -shape.getGlobalBounds().height * 0.4));
			angle = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (PI - 0)));
			break;
		case 3:
			shape.setPosition(Vector2f(rand() % windowSize.x, windowSize.y + shape.getGlobalBounds().height * 0.4));
			angle = PI + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*PI - PI)));
			break;
		}
	}
};