#pragma once
#include <stdio.h>
#include <vector>

class LoadObject
{
private:
	struct Co_ord
	{
		float x;
		float y;
		float z;
		float a;
		float xt;
		float yt;
		float zt;
		float at;
		float xn;
		float yn;
		float zn;
		float an;
	};
public:
	std::vector <Co_ord> end;
	std::vector <Co_ord> nor;
	std::vector <Co_ord> ver;
	std::vector <Co_ord> tex;
	std::vector <Co_ord> face;
	LoadObject(const char*, float);
	~LoadObject();
};

