#pragma once

class Border {
protected:
	std::vector<RectangleShape> borders;

public:
	Border() {
	}

	Border(RenderWindow* window, RectangleShape* shape) {
		for (int i = 0; i < 4; i++)
			borders.push_back(*shape);
		float size = 1;
		borders[0].setSize(Vector2f(window->getSize().x, size));
		borders[0].setPosition(0,0);
		borders[1].setSize(Vector2f(size, window->getSize().y));
		borders[1].setPosition(0, 0);
		borders[2].setSize(Vector2f(window->getSize().x, size));
		borders[2].setPosition(0, window->getSize().y - size);
		borders[3].setSize(Vector2f(10, window->getSize().y));
		borders[3].setPosition(window->getSize().x - size, 0);
	}

	std::vector<RectangleShape> getBorders() {
		return borders;
	}

	void draw(RenderWindow *window) {
		for (int i = 0; i < borders.size(); i++)
			window->draw(borders[i]);
	}
};
