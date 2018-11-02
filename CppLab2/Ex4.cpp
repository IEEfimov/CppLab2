#include "stdafx.h"
#include "Ex4.h"

using namespace std;

Ex4::Ex4()
{
	ExName = (char *) " matrix to matrix ";
}

unsigned long Ex4::doParralel(int cpuCount, int n) {
	int r, c, i;
	cpuCount += 1;
	free(resultP);

	resultP = (float*)malloc(sizeof(float) * n);
	float* mass1 = (float*)malloc(sizeof(float)*n);
	float* mass2 = (float*)malloc(sizeof(float)*n);

	for (int i = 0; i < n; i++) {
		mass1[i] = workMass[i];
		resultP[i] = 0;
	}

	int dim = sqrtl(n);

	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			mass2[i*dim + j] = workMass[j*dim + i];

	auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for shared(mass1, mass2, dim) private(i, c, r) num_threads(cpuCount) schedule(runtime)
	for (r = 0; r < dim; r++) {
		for (c = 0; c < dim; c++) {
			for (i = 0; i < dim; i++) {
				resultP[r*dim + c] += mass1[r*dim + i] * mass2[i*dim + c];
			}
		}
	}

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(mass1);
	free(mass2);
	resultSize = n;

	return duration;
}

unsigned long Ex4::doSingle(int n)
{
	free(resultS);

	resultS = (float*)malloc(sizeof(float) * n);
	float* mass1 = (float*)malloc(sizeof(float)*n);
	float* mass2 = (float*)malloc(sizeof(float)*n);

	for (int i = 0; i < n; i++) {
		mass1[i] = workMass[i];
		mass2[i] = workMass[i];
		resultS[i] = 0;
	}

	int dim = sqrtl(n);
		
	auto start = chrono::high_resolution_clock::now();

	for (int r = 0; r < dim; r++) {
		for (int c = 0; c < dim; c++) {
			for (int i = 0; i < dim; i++) {
				resultS[r*dim + c] += mass1[r*dim + i] * mass2[c*dim + i];
			}
		}
	}

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(mass1);
	free(mass2);
	resultSize = n;
	
	return duration;
}




Ex4::~Ex4()
{
}
