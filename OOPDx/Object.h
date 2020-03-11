#pragma once
class Object
{
public:
	virtual ~Object() = default;
	virtual bool Init() = 0;
	virtual void Dispose() = 0;

protected:
	bool isInited = false;
};

