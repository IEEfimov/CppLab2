#include "stdafx.h"
#include "Ex2.h"

using namespace std;

Ex2::Ex2()
{
	ExName = (char *) " multiple";
}

unsigned long Ex2::writeData(int n, byte toLog, byte isParralel) {
	return Base::writeData(1, toLog, isParralel);
}

unsigned long Ex2::doParralel(int cpuCount, int n) {
	int z, i;
	int iterCount = log2l(n)+1;

	float* mass = (float*)malloc(sizeof(float)*n);
	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
	}

	auto start = chrono::high_resolution_clock::now();

	int chunk = iterCount / cpuCount / 2;

#pragma omp parallel for schedule(runtime) private(i, z) num_threads(cpuCount)
	for (i = 1; i <= iterCount; i++)
	{
		int shag = pow(2, i - 1);
		for (z = shag; z < n; z += 2 * shag)
#pragma omp atomic
			mass[z - shag] += mass[z];
	}
	
	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(resultP);
	resultP = (float*)malloc(sizeof(float) * 1);
	resultP[0] = mass[0];
	cout << "s = " << resultP[0] << "\n";
	free(mass);

	return duration;
}

unsigned long Ex2::doSingle(int n)
{
	int i, step;

	float* mass = (float*)malloc(sizeof(float)*n);
	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
	}

	auto start = chrono::high_resolution_clock::now();

	for (step = 1; step < n; step *= 2)
		for (i = step; i < n; i += 2 * step)
			mass[i - step] += mass[i];

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	resultSize = 1;
	free(resultS);
	resultS = (float*)malloc(sizeof(float) * 1);
	resultS[0] = mass[0];
	cout << "s = " << resultS[0] << "\n";
	free(mass);
	return duration;
}




Ex2::~Ex2()
{
}
