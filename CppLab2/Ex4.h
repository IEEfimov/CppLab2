#pragma once
#include "Base.h"
#include <cmath>
class Ex4 :
	public Base
{
public:
	Ex4();
	~Ex4();

	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
};

