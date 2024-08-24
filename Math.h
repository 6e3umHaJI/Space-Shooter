#pragma once
#include "Constants.h"

float rotationAngle(float currAngle, float newAngle, float rotation) {
	float res = newAngle - currAngle;
	if (rotation > abs(res))
		return newAngle;
	res = 2 * std::atan(std::tan((currAngle - newAngle + PI) / 2));
	if (res > 0)
	{
		res = currAngle + rotation;
		if (res > PI)
			res = res - 2 * PI;
	}
	else
	{
		res = currAngle - rotation;
		if (res < -PI)
			res = res + 2 * PI;
	}
	return res;
}