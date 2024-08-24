#pragma once
#include "Animator.h"

class Animations {
protected:
	std::vector<Animator> animations;

public:
	
	std::vector<Animator> getAnimations() {
		return animations;
	}

	void Update(Time currTime) {
		for (int i = 0; i < animations.size(); i++)
		{
			animations[i].update(currTime);
			if (animations[i].isEndAnim())
				animations.erase(i + animations.begin());
		}
	}

	void addAnim(Animator animator) {
		animations.push_back(animator);
	}

	void clear() {
		animations.clear();
	}
};