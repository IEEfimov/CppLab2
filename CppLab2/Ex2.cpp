#include "stdafx.h"
#include "Ex2.h"

using namespace std;

Ex2::Ex2()
{
	ExName = (char *) " multiple";
}

unsigned long Ex2::doParralel(int cpuCount, int n) {
	int z, i;
	int iterCount = log2l(n)+1;
	auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static, n/2) private(i, z) num_threads(cpuCount)
	//for (step = 1; step < n; step *= 2)
	for (i = 1; i <= iterCount; i++)
	{
		int shag = pow(2, i - 1);
		for (z = shag; z < n; z += 2 * shag)
			mass[z - shag] += mass[z];
	}
		cout << "s = " << mass[0] << endl;
	
	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	return duration;
}

unsigned long Ex2::doSingle(int n)
{
	int i, step;

	auto start = chrono::high_resolution_clock::now();

	for (step = 1; step < n; step *= 2)
		for (i = step; i < n; i += 2 * step)
			mass[i - step] += mass[i];
	cout << "s = " << mass[0] << endl;

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	singleSize = 1;
	for (int i = 0; i < singleSize; i++) singleMass[i] = mass[i];
	return duration;
}




Ex2::~Ex2()
{
}
