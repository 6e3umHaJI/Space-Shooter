#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

class Object {
protected:
	Texture* texture;
	Sprite shape;

public:
	Object() {
		texture = nullptr;
	}

	Object(Texture* texture) {
		this->texture = texture;
		shape.setTexture(*texture);
		shape.setPosition(0, 0);
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2);
	}

	Object(Texture* texture, Vector2f position, float scale) {
		this->texture = texture;
		shape.setTexture(*texture);
		shape.setPosition(position);
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2);
		shape.setScale(scale, scale);
	}

	Sprite getShape() {
		return shape;
	}
	Vector2f getPosition() {
		return shape.getPosition();
	}

	void setTexture(Texture* texture) {
		this->texture = texture;
		shape.setTexture(*texture);
	}
	void setPosition(Vector2f position) {
		shape.setPosition(position);
	}
	void setScale(float scale) {
		shape.setScale(scale, scale);
	}
	void setRotation(float angle){
		shape.setRotation(angle);
	}
	void drawObject(RenderWindow* window) {
		window->draw(shape);
	}
};