#pragma once
#include "Base.h"
#include <cmath>
class Ex2 :
	public Base
{
public:
	Ex2();
	unsigned long writeData(int n, byte toLog, byte isParralel);
	~Ex2();

	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
};

