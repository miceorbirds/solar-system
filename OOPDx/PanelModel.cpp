#include "PanelModel.h"

PanelModel::PanelModel()
{
	curLoc[0] = 0;
	curLoc[1] = -1;
	dir[0] = 0;
	dir[1] = 0;
	vertices[0] = { 0.02f, 0.15f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[1] = { 0.02f, -0.15, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[2] = { -0.02f, -0.15f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[3] = { -0.02f, 0.15f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	for (auto i = 0; i < 4; i++) {
		vertices[i].X += -1;
	}
}

PanelModel::PanelModel(int side)
{
	curLoc[0] = side;
	curLoc[1] = 0;
	dir[0] = 0;
	dir[1] = 0;
	vertices[0] = { 0.02f, 0.15f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[1] = { 0.02f, -0.15, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[2] = { -0.02f, -0.15f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[3] = { -0.02f, 0.15f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	for (auto i = 0; i < 4; i++) {
		vertices[i].X += side;
	}
}

void PanelModel::updatePositionByClick(float x, float y)
{
	if (curLoc[1] <= 0.85 && y>0 || curLoc[1] >= -0.85 && y<0) {
		curLoc[1] +=y;
		for (auto i = 0; i < 4; i++) {
			vertices[i].Y += y;
		}
	}
}

void PanelModel::UpdatePositionByDirection()
{
}
