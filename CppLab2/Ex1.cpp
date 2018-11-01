#include "stdafx.h"
#include "Ex1.h"

using namespace std;

Ex1::Ex1()
{
	ExName = (char *) " multiple";

}

unsigned long Ex1::writeData(int n, byte toLog, byte isParralel) {
	return Base::writeData(3, toLog, isParralel);
}

unsigned long Ex1::doParralel(int cpuCount, int n) {
	int i;
	int countPlus = 0;
	int countMinus = 0;
	int zeroCount = 0;
	cpuCount += 1;

	float* mass = (float*)malloc(sizeof(float)*n);
	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
	}

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
	free(mass);
	free(resultP);
	resultP = (float*) malloc(sizeof(float) * 3);
	resultP[0] = (float)countPlus;
	resultP[1] = (float)countMinus;
	resultP[2] = (float)zeroCount;
	resultSize = 3;

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

	float* mass = (float*)malloc(sizeof(float)*n);
	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
	}

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

	free(mass);
	free(resultS);
	resultS = (float*)malloc(sizeof(float) * 3);
	resultS[0] = (float)countPlus;
	resultS[1] = (float)countMinus;
	resultS[2] = (float)zeroCount;
	resultSize = 3;

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	return duration;
}


Ex1::~Ex1()
{
}
