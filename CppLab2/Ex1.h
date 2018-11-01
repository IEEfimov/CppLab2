#pragma once
#include "Base.h";

#define KOEF 4.0f

class Ex1 : public Base
{
public:
	Ex1();
	~Ex1();

	unsigned long writeData(int n, byte toLog, byte isParralel);
	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);

};

