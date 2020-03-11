#pragma once
#include <iostream>
#include "Object.h"

class Input : public Object
{
public:
	Input()	= default;;
	Input(const Input& input) : Object(input) {  }
	~Input();


	bool Init() override;
	void Dispose() override;;

	void KeyDown(unsigned int key);;
	void KeyUp(unsigned int key);;

	bool IsKeyPressed(unsigned int key);;
	
private:
	bool m_keys[256]{};
};

