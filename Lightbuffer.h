#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H

#include "DirLight.h"
#include "PointLight.h"
#include <vector>

using namespace std;

class Lightbuffer {
private:
	vector<DirLight> buffer_Dir;
	vector<PointLight> buffer_Point;
public:
	Lightbuffer(){}
	void addLight(const DirLight& light)
	{
		buffer_Dir.push_back(light);
	}
	void addLight(const PointLight& light)
	{
		buffer_Point.push_back(light);
	}
	int dirSize()
	{
		return buffer_Dir.size();
	}
	int pointSize()
	{
		return buffer_Point.size();
	}
	DirLight& getDirLight(int i)
	{
		return buffer_Dir[i];
	}
	PointLight& getPointLight(int i)
	{
		return buffer_Point[i];
	}
};

#endif // !LIGHTBUFFER_H
