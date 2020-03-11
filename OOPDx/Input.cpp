#include "Input.h"

Input::~Input()
{
	Dispose();
}

bool Input::Init()
{
	for (bool& m_key : m_keys)
	{
		m_key = false;
	}

	std::cout << "Init " << typeid(this).name() << " Class" << std::endl;
	isInited = true;
	
	return isInited;
}

void Input::Dispose()
{
	if(!isInited) return;
		
	std::cout << "Destroy " << typeid(this).name() << " Class" << std::endl;

	isInited = false;
}

void Input::KeyDown(unsigned key)
{
	if (!isInited) return;
	
	//std::cout << typeid(this).name() << " Down " << key << std::endl;
	
	m_keys[key] = true;
}

void Input::KeyUp(unsigned key)
{
	if (!isInited) return;
	
	//std::cout << typeid(this).name() << " Up " << key << std::endl;

	m_keys[key] = false;
}

bool Input::IsKeyPressed(unsigned key)
{
	return m_keys[key];
}
