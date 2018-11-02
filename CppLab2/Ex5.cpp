#include "stdafx.h"
#include "Ex5.h"

using namespace std;

Ex5::Ex5()
{
	ExName = (char *) " matrix to matrix ";
}

unsigned long Ex5::writeData(int n, byte toLog, byte isParralel) {
	return Base::writeData(1, toLog, isParralel);
}

unsigned long Ex5::doParralel(int cpuCount, int n) {
	int i;
	float minimal;

	cpuCount += 1;
	free(resultP);
	resultP = (float*)malloc(sizeof(float));
	
	float* mass = (float*)malloc(sizeof(float)*n);
	memcpy(mass, workMass, sizeof(float)*n);

	int chunk = n / cpuCount;
	float* thrMinimals = (float*)malloc(sizeof(float)*cpuCount);

	auto start = chrono::high_resolution_clock::now();

#pragma omp parallel num_threads(cpuCount) shared(chunk)
	{
		int thrNum = omp_get_thread_num();
		float minimal = mass[chunk*thrNum];

		for (int i = thrNum*chunk; i < chunk*(thrNum+1); i++)
		{
			if (mass[i] < minimal)
				minimal = mass[i];
		}
		thrMinimals[thrNum] = minimal;
	}
	minimal = thrMinimals[0];
	for (i = 0; i < cpuCount; i++)
	{
		if (thrMinimals[i] < minimal)
			minimal = thrMinimals[i];
	}

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(mass);
	resultP[0] = minimal;
	resultSize = 1;

	return duration;
}

unsigned long Ex5::doSingle(int n)
{
	int minimal;
	free(resultS);

	resultS = (float*)malloc(sizeof(float));
	float* mass = (float*)malloc(sizeof(float)*n);

	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
	}

	auto start = chrono::high_resolution_clock::now();

	minimal = mass[0];
	for (int i = 0; i < n; i++)
	{
		if (mass[i] < minimal)
			minimal = mass[i];
	}

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(mass);
	resultS[0] = minimal;

	return duration;
}




Ex5::~Ex5()
{
}
