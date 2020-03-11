#pragma once
struct Color
{
	

public:
	Color() {  }

	Color(float r, float g, float b, float a) : red(r), green((g)), blue(b), alpha(a) {  };
	Color(float r, float g, float b) : red(r), green((g)), blue(b), alpha(1) {  };
	
	float GetRed() const { return red; }
	float GetGreen() const { return green; }
	float GetBlue() const { return blue; }
	float GetAlpha() const { return alpha; }
	
private:
	float red;
	float green;
	float blue;
	float alpha;
};

static  Color Red(1, 0, 0, 1);
static  Color Green(0, 1, 0, 1);
static  Color Blue(0, 0, 1, 1);

