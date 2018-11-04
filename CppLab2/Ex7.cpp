#include "stdafx.h"
#include "Ex7.h"

using namespace std;

Ex7::Ex7()
{
	ExName = (char *) " quick sort ";
}

void quicksort(float *, int, int);
int partition(float *, int, int);
int choosePivot(float *, int, int);
void swap(float *, float *);



void quicksort(float * arr, int lo, int hi)
{
	if (lo < hi)
	{
		int p = partition(arr, lo, hi);
		quicksort(arr, lo, p - 1);
		quicksort(arr, p + 1, hi);
	}
}

int partition(float* arr, int lo, int hi)
{
	int i;
	int pivotIdx = choosePivot(arr, lo, hi);
	int pivotVal = arr[pivotIdx];

	swap(&arr[pivotIdx], &arr[hi]);

	int storeIdx = lo;

	for (i = lo; i < hi; i++)
	{
		if (arr[i] < pivotVal)
		{
			swap(&arr[i], &arr[storeIdx]);
			storeIdx++;
		}
	}

	swap(&arr[storeIdx], &arr[hi]);
	return storeIdx;
}

void swap(float * x, float * y)
{
	float temp = *x;
	*x = *y;
	*y = temp;
}

//Select the median of arr[lo], arr[hi], and arr[(lo+hi)/2]
int choosePivot(float * arr, int lo, int hi)
{
	int mid = (lo + hi) / 2;

	int temp;
	if (arr[lo] > arr[hi])
	{
		temp = lo;
		lo = hi;
		hi = temp;
	}
	if (arr[mid] < arr[lo])
	{
		temp = mid;
		mid = lo;
		lo = temp;
	}
	if (arr[hi] < arr[mid])
	{
		temp = mid;
		mid = hi;
		hi = temp;
	}
	return mid;
}

//==============================================================

//void Ex7::quickSortParallel(float mass[], int left, int right) {
//	int i = left, j = right;
//	float tmp;
//	int pivot = mass[(left + right) / 2];
//
//	/* partition */
//	while (i <= j) {
//		while (mass[i] < pivot)
//			i++;
//		while (mass[j] > pivot)
//			j--;
//		if (i <= j) {
//			tmp = mass[i];
//			mass[i] = mass[j];
//			mass[j] = tmp;
//			i++;
//			j--;
//		}
//	};
//
//	/* recursion */
//#pragma omp sections
//	{
//#pragma omp section
//		if (left < j)
//			quickSortSingle(mass, left, j);
//#pragma omp section
//		if (i < right)
//			quickSortSingle(mass, i, right);
//	}
//
//}

/* How many workers can be working at same time */
int num_of_workers = 0;
/* How many active workers currently */
int num_of_active_workers = 0;

omp_lock_t * worker_lock;

void _qsort(float *data, int n, int was_spawned)
{
	if (n >  1) {

		//	printf("Thread id %d %d\n", omp_get_thread_num(), n);

		float pivot = data[0];
		int l = 0;
		int r = n;

		while (1)
		{
			while (l < n && data[++l] < pivot);
			while (data[--r] > pivot);
			if (l >= r)
				break;
			int t = data[l];
			data[l] = data[r];
			data[r] = t;
		}

		float t = data[l - 1];
		data[l - 1] = data[0];
		data[0] = t;

		int recursion_in_parallel = 0;
		omp_set_lock(worker_lock);

		/* Limit number of threads running */
		if (num_of_active_workers < num_of_workers) {
			recursion_in_parallel = 1;
			num_of_active_workers += 2;
		}
		omp_unset_lock(worker_lock);

		if (recursion_in_parallel == 1) {
#pragma omp parallel sections
			{
#pragma omp section
				_qsort(data, l - 1, 1);

#pragma omp section
				_qsort(data + l, n - l, 1);
			}
		}
		else {
			_qsort(data, l - 1, 0);
			_qsort(data + l, n - l, 0);
		}
	}
	/* Decrease num */
	if (was_spawned == 1) {
		omp_set_lock(worker_lock);
		num_of_active_workers--;
		omp_unset_lock(worker_lock);
	}
}

//void Ex7::quickSortSingle(float mass[], int left, int right) {
//	int i = left, j = right;
//	float tmp;
//	int pivot = mass[(left + right) / 2];
//
//	/* partition */
//	while (i <= j) {
//		while (mass[i] < pivot)
//			i++;
//		while (mass[j] > pivot)
//			j--;
//		if (i <= j) {
//			tmp = mass[i];
//			mass[i] = mass[j];
//			mass[j] = tmp;
//			i++;
//			j--;
//		}
//	};
//
//	/* recursion */
//	if (left < j)
//		quickSortSingle(mass, left, j);
//	if (i < right)
//		quickSortSingle(mass, i, right);
//}



unsigned long Ex7::doParralel(int cpuCount, int n) {
	cpuCount += 1;
	free(resultP);
	resultP = (float*)malloc(sizeof(float)*n);

	float* mass = (float*)malloc(sizeof(float)*n);
	memcpy(mass, workMass, sizeof(float)*n);

	auto start = chrono::high_resolution_clock::now();

	worker_lock = (omp_lock_t*)malloc(sizeof(omp_lock_t));
	omp_init_lock(worker_lock);

	omp_set_num_threads(cpuCount);
	num_of_workers = cpuCount;
	num_of_active_workers = 1;
	_qsort(mass, n, 1);


	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	memcpy(resultP, mass, (sizeof(float)*n));
	free(mass);
	resultSize = n;

	return duration;
}

unsigned long Ex7::doSingle(int n)
{
	int i, j;
	bool swapped;

	free(resultS);
	resultS = (float*)malloc(sizeof(float)*n);
	float* mass = (float*)malloc(sizeof(float)*n);

	memcpy(mass, workMass, (sizeof(float)*n));

	auto start = chrono::high_resolution_clock::now();

	quicksort(mass, 0, n - 1);

	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();

	memcpy(resultS, mass, (sizeof(float)*n));
	free(mass);
	resultSize = n;


	return duration;
}


Ex7::~Ex7()
{
}



