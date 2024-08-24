#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Constants.h"
using namespace sf;
class NicknameForm {
protected:
	RectangleShape background;
	Text text1, textScore, text2, nickname;
	std::string str;
	int score;
	bool isActive;
	Font *font,*fontNickname;

public:
	NicknameForm() {
		background.setSize(Vector2f(900, 450));
		background.setFillColor(Color(107, 107, 107,200));
		background.setOrigin(background.getLocalBounds().width * 0.5, background.getLocalBounds().height * 0.5);
		background.setPosition(1920 * 0.5, 1080 * 0.5 - 100);
		font = new Font;
		font->loadFromFile("fonts/Roboto.ttf");
		fontNickname = new Font;
		fontNickname->loadFromFile("fonts/SignikaNegative.ttf");
		Text text("", *font, 50);
		text.setFillColor(Color::White);
		text1 = text;
		text1.setString("Your score:");
		text1.setOrigin(text1.getLocalBounds().width * 0.5, 0);
		text1.setFillColor(Color(240, 240, 240, 200));
		int space = (background.getSize().y - 4 * text1.getGlobalBounds().height) / 5;
		Vector2f position = background.getPosition();
		position.y = position.y - background.getGlobalBounds().height*0.5+space-30;
		space += text1.getGlobalBounds().height;
		text1.setPosition(position);
		textScore = text;
		textScore.setFont(*fontNickname);
		textScore.setPosition(position.x, position.y + space);
		textScore.setString("");
		text2 = text;
		text2.setPosition(position.x, position.y+2*space);
		text2.setString("Enter your nickname:");
		text2.setOrigin(text2.getLocalBounds().width * 0.5, 0);
		text2.setFillColor(Color(240, 240, 240, 200));
		nickname = text;
		nickname.setFont(*fontNickname);
		nickname.setPosition(position.x, position.y + 3*space);
		nickname.setString("");
		score = 0;

		isActive = false;
	}

	void setScore(int score) {
		this->score = score;
		std::ostringstream strScore;
		strScore << score;
		textScore.setString(strScore.str());
		textScore.setOrigin(textScore.getLocalBounds().width * 0.5, 0);
	}

	void activate(){
		isActive = true;
	}

	bool isActivate() {
		return isActive;
	}

	void reset() {
		score = 0;
		textScore.setString("");
		nickname.setString("");
		isActive = false;
		str.clear();
	}

	void update(RenderWindow* window, Record *record) {
		Event event;
		strcpy_s(record->name, "");
		while(window->pollEvent(event))
		if (event.type == sf::Event::TextEntered)
		{
			switch (event.text.unicode) {
			case 27:
				reset();
				break;
			case 8:
				if (!str.empty())
					str.pop_back();
				nickname.setString(str);
				nickname.setOrigin(nickname.getLocalBounds().width * 0.5, 0);
				break;
			case 13:
				strcpy_s(record->name, str.c_str());
				record->score = score;
				reset();
				break;
			default:
				if (event.text.unicode <= 126 && event.text.unicode >= 33 && str.size() < 14)// отсекаем не ASCII символы
				{
					str += static_cast<char>(event.text.unicode);
					nickname.setString(str);
					nickname.setOrigin(nickname.getLocalBounds().width * 0.5, 0);
				}
			}
		}
	}
	
	void draw(RenderWindow* window) {
		window->draw(background);
		window->draw(text1);
		window->draw(textScore);
		window->draw(text2);
		window->draw(nickname);
	}
};