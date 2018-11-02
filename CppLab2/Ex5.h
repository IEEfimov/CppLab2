#pragma once
#include "Base.h"
#include <cmath>
class Ex5 :
	public Base
{
public:
	Ex5();
	unsigned long writeData(int n, byte toLog, byte isParralel);
	~Ex5();

	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
};

