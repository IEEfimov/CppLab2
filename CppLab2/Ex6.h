#pragma once
#include "Base.h"
#include <cmath>
class Ex6 :
	public Base
{
public:
	Ex6();
	void swap(float * xp, float * yp);
	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
	~Ex6();

	
};

