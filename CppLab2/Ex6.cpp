#include "stdafx.h"
#include "Ex6.h"

using namespace std;

Ex6::Ex6()
{
	ExName = (char *) " matrix to matrix ";
}

void Ex6::swap(float *xp, float *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}


unsigned long Ex6::doParralel(int cpuCount, int n) {
	int i, j;
	bool swapped;

	cpuCount += 1;
	free(resultP);
	resultP = (float*)malloc(sizeof(float)*n);

	float* mass = (float*)malloc(sizeof(float)*n);
	memcpy(mass, workMass, sizeof(float)*n);

	auto start = chrono::high_resolution_clock::now();

	for (i = 0; i < n; i++) {
		int first = i % cpuCount;
		
#pragma omp parallel for shared(mass, first) num_threads(cpuCount)
		for (int j = first; j < n - 1; j += 2)
		{
			if (mass[j] > mass[j + 1])
			{
				swap(&mass[j], &mass[j + 1]);
			}
		}
	}
	

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	memcpy(resultP, mass, (sizeof(float)*n));
	free(mass);
	resultSize = n;

	return duration;
}

unsigned long Ex6::doSingle(int n)
{
	int i, j;
	bool swapped;
	
	free(resultS);
	resultS = (float*)malloc(sizeof(float)*n);
	float* mass = (float*)malloc(sizeof(float)*n);

	memcpy(mass, workMass, (sizeof(float)*n));

	auto start = chrono::high_resolution_clock::now();

	for (i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (mass[j] > mass[j + 1])
			{
				swap(&mass[j], &mass[j + 1]);
				swapped = true;
			}
		}

		if (swapped == false)
			break;
	}

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	memcpy(resultS, mass, (sizeof(float)*n));
	free(mass);
	resultSize = n;
	

	return duration;
}


Ex6::~Ex6()
{
}
