#pragma once
#include "Base.h"
#include <cmath>
class Ex7 :
	public Base
{
public:
	Ex7();
	void quickSortParallel(float mass[], int left, int right);
	void quickSortSingle(float mass[], int left, int right);
	unsigned long doParralel(int cpuCount, int n);
	unsigned long doSingle(int n);
	~Ex7();


};

