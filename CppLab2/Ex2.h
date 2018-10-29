#pragma once
#include "Base.h"
#include <cmath>
class Ex2 :
	public Base
{
public:
	Ex2();
	~Ex2();

	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
};

