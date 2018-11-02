#pragma once
#include "Base.h"
#include <cmath>
class Ex3 :
	public Base
{
public:
	Ex3();
	unsigned long writeData(int n, byte toLog, byte isParralel);
	~Ex3();

	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
};

