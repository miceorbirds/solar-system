#pragma once
#include "ModelClass.h"
class PanelModel :
	public ModelClass
{
public:
	PanelModel();
	PanelModel(int side);
	PanelModel(float x, float y) : ModelClass(x, y) {}
	void updatePositionByClick(float, float);
	void UpdatePositionByDirection();
};

