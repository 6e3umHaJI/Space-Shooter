#pragma once

class HealthBar {
protected:
	int health, healthMax;
	float height;
	RectangleShape currHealth, takenHealth;
public:
	HealthBar() {
		this->healthMax = 100;
		health = healthMax;
		height = 10;
		currHealth.setFillColor(Color::Green);
		currHealth.setSize(Vector2f(healthMax, 3));
		takenHealth.setFillColor(Color::Red);
		takenHealth.setSize(Vector2f(healthMax - health, 3));
	}

	int getHealthMax() {
		return healthMax;
	}

	int getHealth() {
		return health;
	}

	RectangleShape getShapeCurrHealth() {
		return currHealth;
	}

	RectangleShape getShapeTakenHealth() {
		return takenHealth;
	}

	void setHealthMax(int healthMax, Sprite shape) {
		height = shape.getGlobalBounds().height;
		this->healthMax = healthMax;
		health = healthMax;
		currHealth.setSize(Vector2f(healthMax, 3));
		takenHealth.setSize(Vector2f(healthMax - health, 3));
		currHealth.setPosition(Vector2f(shape.getPosition().x - 0.5 * healthMax, shape.getPosition().y - 0.4 * height));
		takenHealth.setPosition(Vector2f(shape.getPosition().x - 0.5 * healthMax + health, shape.getPosition().y - 0.4 * height));
	}

	void reduceHealth(int damage) {
		health = health - damage;
		if (health < 0)
			health = 0;
		currHealth.setSize(Vector2f(health, 3));
		takenHealth.setSize(Vector2f(healthMax - health, 3));
	}

	void recoverHealth(int recoverAmount) {
		health = health + recoverAmount;
		if (health > healthMax)
			health = healthMax;
		currHealth.setSize(Vector2f(health, 3));
		takenHealth.setSize(Vector2f(healthMax - health, 3));
	}

	void setPosition(Vector2f pos) {
		currHealth.setPosition(Vector2f(pos.x - 0.5 * healthMax, pos.y - 0.4 * height));
		takenHealth.setPosition(Vector2f(pos.x - 0.5 * healthMax + health, pos.y - 0.4 * height));
	}

	void draw(RenderWindow* window) {
		window->draw(currHealth);
		window->draw(takenHealth);
	}
};