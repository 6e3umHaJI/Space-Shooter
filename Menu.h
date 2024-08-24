#pragma once
#include <iostream>
#include "Object.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"
using namespace sf;

class Menu {
protected:
	Texture buttonResumeTex, buttonRestartTex, buttonQuitTex, buttonExitTex;
	std::vector<Button> button;
	int activeButton;
	bool isActive;

public:

	Menu() {
		buttonResumeTex.loadFromFile("textures/button_menu_resume.png");
		buttonRestartTex.loadFromFile("textures/button_menu_restart.png");
		buttonQuitTex.loadFromFile("textures/button_menu_quit.png");
		buttonExitTex.loadFromFile("textures/button_menu_exit.png");
		float scale = 0.5f;
		int distanceBetween = buttonResumeTex.getSize().y * scale + 20;
		Vector2f position = Vector2f(1920*0.5, 300);
		button.push_back(Button(&buttonResumeTex, position, scale)); // Button play
		button.push_back(Button(&buttonRestartTex, Vector2f(position.x, position.y + distanceBetween), scale)); // Button records
		button.push_back(Button(&buttonQuitTex, Vector2f(position.x, position.y + distanceBetween*2), scale)); // Button rules
		button.push_back(Button(&buttonExitTex, Vector2f(position.x, position.y + distanceBetween*3), scale)); // Button exit
		activeButton = -1;
		isActive = false;
	}

	void open() {
		isActive = true;
	}

	void close() {
		isActive = false;
	}

	bool isActivated() {
		return isActive;
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
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				activeButton = 0;
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