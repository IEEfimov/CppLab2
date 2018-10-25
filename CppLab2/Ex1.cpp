#include "stdafx.h"
#include "Ex1.h"

using namespace std;

Ex1::Ex1()
{
	ExName = (char *) " multiple";

}

unsigned long Ex1::doParralel(int cpuCount, int n) {
	int i;
	int k = KOEF;
	float result = 0;
	cpuCount += 1;

	auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(runtime) private(i) reduction(+:result) num_threads(cpuCount)
	for (i = 0; i < n; i++) {
		mass[i] *= k;
		result = result + mass[i];
	}

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	return duration;
}

unsigned long Ex1::doSingle(int n)
{
	int i;
	int k = KOEF;
	auto start = chrono::high_resolution_clock::now();


	for (i = 0; i < n; i++) {
		singleMass[i] *= k;
	}

	singleSize = n;

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	return duration;
}


Ex1::~Ex1()
{
}
