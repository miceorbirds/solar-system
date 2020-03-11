#pragma once
#include "ModelClass.h"
class BallModel :
	public ModelClass
{
public:
	BallModel();
	BallModel(float x, float y) : ModelClass(x, y) {}
	float getPositionX();
	float getPositionY();
};

