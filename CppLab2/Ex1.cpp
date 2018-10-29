#include "stdafx.h"
#include "Ex1.h"

using namespace std;

Ex1::Ex1()
{
	ExName = (char *) " multiple";

}

unsigned long Ex1::writeData(int n, byte toLog) {
	return Base::writeData(3, toLog);
}

unsigned long Ex1::doParralel(int cpuCount, int n) {
	int i;
	int countPlus = 0;
	int countMinus = 0;
	int zeroCount = 0;
	cpuCount += 1;

	auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static, n/2) private(i) num_threads(cpuCount)
	for (i = 0; i < n; i++) {
		if (mass[i] == 0) {
#pragma omp critical(name1)
			zeroCount++;
		}
		else if (mass[i] > 0) {
#pragma omp critical(name2)
			countPlus++;
		}
		else if (mass[i] < 0) {
#pragma omp critical(name3)
			countMinus++;
		}
	}
	mass[0] = (float)countPlus;
	mass[1] = (float)countMinus;
	mass[2] = (float)zeroCount;
	

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	return duration;
}

unsigned long Ex1::doSingle(int n)
{
	int i;
	int countPlus = 0;
	int countMinus = 0;
	int zeroCount = 0;

	auto start = chrono::high_resolution_clock::now();

	for (i = 0; i < n; i++) {
		if (mass[i] == 0) {
			zeroCount++;
		}else if (mass[i] > 0) {
			countPlus++;
		}else if (mass[i] < 0) {
			countMinus++;
		}
	}
	mass[0] = (float)countPlus;
	mass[1] = (float)countMinus;
	mass[2] = (float)zeroCount;


	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	return duration;
}


Ex1::~Ex1()
{
}
