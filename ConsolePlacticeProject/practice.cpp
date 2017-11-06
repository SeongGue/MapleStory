#include <iostream>
#include <windows.h>

struct Vector2
{
	float x;
	float y;

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

int main()
{
	Vector2 v(1.0f, 2.0f);
	std::cout << sizeof(v) << std::endl;
}
