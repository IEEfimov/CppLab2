#pragma once
#include <conio.h>
#include <windows.h>
#include <omp.h>
#include <chrono>
#include <string>
#include <iostream>

#define MAX_CPU_COUNT 64;
#define MIN_N 1000;
#define MAX_N 10000;
#define STEP_N 1000;
#define APROXIME 10 ; 

#define THR_START 0;
#define THR_END 3 ;
#define THR_STEP 1;



class Base
{
public:
	Base();
	~Base();

	char* ExName;

	float* workMass;

	int resultSize = 0;
	float* resultP;
	float* resultS;

	
	virtual unsigned long doParralel(int cpuCount, int n);
	virtual unsigned long doSingle(int n);

	unsigned long wrapAlgo(int cpuCount, int n);

	void printExName(int cpuCount, int n);

	unsigned long readData(int n, byte toLog);
	unsigned long createData(int n, byte toLog);
	virtual unsigned long writeData(int n, byte toLog, byte isParralel);

	void doAnalise();
	void saveResults(unsigned long * res, unsigned long* write, unsigned long* read);
	void showResults(unsigned long* run, unsigned long* read, unsigned long* write);

	bool isTheSame();

};

