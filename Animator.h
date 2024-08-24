#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Animator {
protected:
	Sprite shape;
	Texture* texture;
	int frameNum;
	int traccia;
	int lastFrame;
	Vector2i frameSize;
	float animationTime;
	std::vector<sf::IntRect> frames;
	bool loop, endAnim = false;
	Time timeBegin;
	bool dispel;
	Color color;

public: 
	Animator(Texture *texture, Vector2f pos, int frameNum, int traccia, Vector2i frameSize, float animationTime, bool loop, float scale, bool isDispel) {
		this->texture = texture;
		shape.setPosition(pos);
		shape.setTexture(*texture);
		shape.setTextureRect(IntRect(0,0,frameSize.x, frameSize.y));
		shape.setOrigin(frameSize.x * 0.5, frameSize.y * 0.5);
		this->frameNum = frameNum;
		this->traccia = traccia;
		lastFrame = 0;
		this->frameSize = frameSize;
		this->animationTime = animationTime;
		this->loop = loop;
		addFrames();
		setScale(scale);
		//this->timeBegin = timeBegin;
		timeBegin = microseconds(0);
		dispel = isDispel;
		color = shape.getColor();
	}

	Sprite getShape() {
		return shape;
	}
	Texture* getTexture() {
		return texture;
	}
	int getFrameNum() {
		return frameNum;
	}
	int getTraccia() {
		return traccia;
	}
	Vector2i getFrameSize() {
		return frameSize;
	}
	float getAnimationTime() {
		return animationTime;
	}

	void setScale(float scale) {
		shape.setScale(scale,scale);
	}

	void setTexture(Texture *texture) {
		this->texture = texture;
	}
	void setFrameNum(int frameNum) {
		this->frameNum = frameNum;
	}
	void setTraccia(int traccia) {
		this->traccia = traccia;
	}
	void setFrameSize(Vector2i frameSize) {
		this->frameSize = frameSize;
	}
	void setAnimationTime(float animationTime) {
		this->animationTime = animationTime;
	}

	void setPosition(Vector2f pos) {
		shape.setPosition(pos);
	}

	void addFrames()
	{
		Vector2i current = Vector2i(0,0);
		for (unsigned int t = 0; t < traccia; t++) {
			for (unsigned int i = 0; i < frameNum; i++)
			{
				frames.push_back(IntRect(current.x, current.y, frameSize.x, frameSize.y));
				current.x += frameSize.x;
			}
			current.y += frameSize.y;
			current.x = 0;
		}
	}

	void update(Time currTime)
	{
		int frameAmount = frameNum * traccia;
		if (frames.size() == 0) return;
		if (lastFrame == 0)
		{
			timeBegin = currTime;
			lastFrame += 1;
		}
		float delta = currTime.asSeconds() - timeBegin.asSeconds();
		if (delta < 0)
			delta += TIME_CYCLE/1000;
		auto currentFrame = static_cast<int>(delta / animationTime * static_cast<float>(frameAmount));
		if (loop) currentFrame %= frameAmount;
		else if (currentFrame >= frameAmount) {
				currentFrame = frameAmount - 1;
				endAnim = true; }
		shape.setTextureRect(frames[currentFrame]);
		if (dispel)
		{
			shape.setColor(color - Color(0, 0, 0, 255/frameAmount*currentFrame));
		}
	}

	bool isEndAnim() {
		return endAnim;
	}
};