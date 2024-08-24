#pragma once
#include "Object.h"
#include <string>

class Button: public Object {
protected:
	Color color;
	bool isActive;

public:
	Button(Texture* texture, Vector2f position, float scale) :Object(texture, position, scale) {
		color = shape.getColor();
		isActive = false;
	}

	void setVisibility(unsigned char alpha) {
		shape.setColor(this->color - Color(0,0,0,alpha));
	}

	void activate() {
		setVisibility(100);
		isActive = true;
	}

	void hover() {
		setVisibility(50);
		isActive = false;
	}

	void reset() {
		setVisibility(0);
		isActive = false;
	}

	bool isActivated() {
		return isActive;
	}

};