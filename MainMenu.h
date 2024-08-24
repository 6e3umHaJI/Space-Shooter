#pragma once
#include <iostream>
#include "Object.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"
using namespace sf;

class MainMenu {
protected:
	Texture buttonExitTex, buttonPlayTex, buttonRecordsTex, buttonRulesTex;
	std::vector<Button> button;
	int activeButton;


public:

	MainMenu() {
		buttonPlayTex.loadFromFile("textures/button_play.png");
		buttonExitTex.loadFromFile("textures/button_exit.png");
		buttonRecordsTex.loadFromFile("textures/button_records.png");
		buttonRulesTex.loadFromFile("textures/button_rules.png");
		float scale = 0.4f;
		int distanceBetween = buttonPlayTex.getSize().x * scale + 5;
		Vector2f position = Vector2f(650, 850);
		button.push_back(Button(&buttonPlayTex, position, scale)); // Button play
		button.push_back(Button(&buttonRecordsTex, Vector2f(position.x + distanceBetween, position.y), scale)); // Button records
		button.push_back(Button(&buttonRulesTex, Vector2f(position.x + distanceBetween * 2, position.y), scale)); // Button rules
		button.push_back(Button(&buttonExitTex, Vector2f(position.x + distanceBetween * 3, position.y), scale)); // Button exit
		activeButton = -1;
	}

	void activateButton(char buttonIndex) {
		for (int index = 0; index < button.size(); index++)
		{
			if (index == buttonIndex)
				button[index].activate();
			else
				button[index].reset();
		}
		activeButton = buttonIndex;
	}

	void reset() {
		for (int index = 0; index < button.size(); index++)
		{
			button[index].reset();
		}
		activeButton = -1;
	}

	int getActiveButton() {
		return activeButton;
	}

	int update(RenderWindow* window) {
		for (int index = 0; index < button.size(); index++)
		{
			if (button[index].isActivated() == false) {
				if (IntRect((button[index]).getShape().getGlobalBounds()).contains(Mouse::getPosition(*window))) {
					if (Mouse::isButtonPressed(Mouse::Left)) {
						activeButton = index;
						activateButton(index);
					}
					else button[index].hover();
				}
				else button[index].reset();
			}
		}
		if (button[1].isActivated())
			return activeButton;
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		return activeButton;
	}

	void draw(RenderWindow* window) {
		for (int index = 0; index < button.size(); index++)
		{
			button[index].drawObject(window);
		}
	}
};