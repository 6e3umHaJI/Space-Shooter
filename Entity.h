#pragma once
#include "Object.h"
#include "HealthBar.h"

class Entity:public Object {
protected:
	float speed;
	HealthBar healthBar;
	RectangleShape body;
	bool dead;
	int score;

public:
	Entity(Texture* texture, Vector2f position, float speed, float scale, int healthMax, int score) :Object(texture, position, scale)
	{
		this->speed = speed;
		healthBar.setHealthMax(healthMax, shape);
		body.setSize(Vector2f(texture->getSize().x*scale*0.4, texture->getSize().y*scale*0.4));
		body.setOrigin(body.getLocalBounds().width / 2, body.getLocalBounds().height / 2);
		body.setPosition(position);
		body.setFillColor(Color::Red);
		dead = false;
		this->score = score;
	}
	Entity():Object() {

	}

	int getSpeed() {
		return speed;
	}

	int getScore() {
		return score;
	}

	void setBodyScale(Vector2f scale) {
		body.setSize(Vector2f(body.getSize().x*scale.x, body.getSize().y * scale.y));
	}

	bool isDead() {
		if (healthBar.getHealth() == 0)
			dead = true;
		return dead;
	}

	RectangleShape getBody() {
		return body;
	}

	void setSpeed(int speed) {
		this->speed = speed;
	}

	void setPositionBody(Vector2f position) {
		body.setPosition(position);
	}

	void reduceHealth(int damage) {
		healthBar.reduceHealth(damage);
		healthBar.setPosition(shape.getPosition());
	}
	
	void moveBody() {
		body.setPosition(shape.getPosition());
	}

	void rotateBody() {
		body.setRotation(shape.getRotation());
	}

	virtual void spawn(Vector2i window) {
	}

	void drawEntity(RenderWindow* window) {
		healthBar.setPosition(shape.getPosition());
		window->draw(shape);
		healthBar.draw(window);
		//window->draw(body);
	}
};