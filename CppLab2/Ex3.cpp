#include "stdafx.h"
#include "Ex3.h"

using namespace std;

Ex3::Ex3()
{
	ExName = (char *) " matrix to matrix ";
}

unsigned long Ex3::writeData(int n, byte toLog, byte isParralel) {
	return Base::writeData(1, toLog, isParralel);
}

unsigned long Ex3::doParralel(int cpuCount, int n) {
	long log_of_N = (long)ceil(log(n) / log(2));
	float ** scanSum = (float **)malloc(sizeof(float *) * 2);
	scanSum[0] = (float *)malloc(sizeof(float)*n);
	scanSum[1] = (float *)malloc(sizeof(float)*n);

	int i, j;
	int in = 1, out = 0;

	cpuCount += 1;
	free(resultP);

	resultP = (float*)malloc(sizeof(float) * n);
	float* mass = (float*)malloc(sizeof(float)*n);

	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
		resultP[i] = 0;
	}

	memcpy(scanSum[out], mass, (n) * sizeof(float));
	memset(scanSum[in], 0, (n) * sizeof(float));

	auto start = chrono::high_resolution_clock::now();

	int two_i = 0, two_i_1 = 0;

	for (i = 1; i <= log_of_N; i++)
	{
		two_i_1 = 1 << (i - 1);
		two_i = 1 << i;
		out = 1 - out;
		in = 1 - out;
#pragma omp parallel for private(j) shared(scanSum, in, out) num_threads(cpuCount)
		for (j = 0; j < n; j++)
		{
			if (j >= two_i_1)
				scanSum[out][j] = scanSum[in][j] + scanSum[in][j - two_i_1];
			else
				scanSum[out][j] = scanSum[in][j];
		}
	}

	for (i = 0; i < n; i++)
		resultP[i] = scanSum[out][i];

	free((void *)scanSum[0]);
	free((void *)scanSum[1]);
	free((void *)scanSum);
	free((void *)mass);


	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(mass);
	resultSize = n;

	return duration;
}

unsigned long Ex3::doSingle(int n)
{
	free(resultS);

	resultS = (float*)malloc(sizeof(float) * n);
	float* mass = (float*)malloc(sizeof(float)*n);

	for (int i = 0; i < n; i++) {
		mass[i] = workMass[i];
		resultS[i] = 0;
	}

	auto start = chrono::high_resolution_clock::now();

	for (int i = 1; i < n; i++)
		resultS[i] = resultS[i - 1] + mass[i];

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	free(mass);
	resultSize = n;

	return duration;
}




Ex3::~Ex3()
{
}
