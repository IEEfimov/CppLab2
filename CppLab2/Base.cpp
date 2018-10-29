#include "stdafx.h"
#include "Base.h"

using namespace std;



Base::Base()
{
}
unsigned long Base::readData(int n, byte toLog) {
	FILE *f;
	/*string fileName = FOLDER_NAME;
	if (CreateDirectory(fileName.c_str() , NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	else return -1;*/


	string fileName = "input_";
	fileName.append(to_string(n));
	fileName.append(".bin");
	if (toLog) printf_s("Starting to read the file %s...  ", fileName.c_str());
	auto start = chrono::high_resolution_clock::now();
	int err = fopen_s(&f, fileName.c_str(), "rb");
	if (err == 0) {
		for (int i = 0; i < n; i++) fscanf_s(f, "%f", &mass[i]);
		fclose(f);
	}
	else if (err == 2) {
		puts("\n\t\tThe file doesn`t exist. Create a new one...\n");
		createData(n, true);
	}
	else {
		puts("\n\t\tCannot read the file!");
		_getch();
		exit(4);
	}
	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
	if (toLog) printf_s("Done in %dms \n", duration);
	return duration;

}
unsigned long Base::createData(int n, byte toLog) {
	FILE *f;
	/*string fileName = FOLDER_NAME;
	if (CreateDirectory(fileName.c_str() , NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	else return -1;*/


	string fileName = "input_";
	fileName.append(to_string(n));
	fileName.append(".bin");
	auto start = chrono::high_resolution_clock::now();
	int err = fopen_s(&f, fileName.c_str(), "wb");
	if (err == 0) {
		for (int i = 0; i < n; i++) {
			fprintf_s(f, "%d ", i % 10);
			mass[i] = i % 10;
		}
		fclose(f);
	}
	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
	return duration;

}
unsigned long Base::writeData(int n, byte toLog) {
	FILE *f;
	/*string fileName = FOLDER_NAME;
	if (CreateDirectory(fileName.c_str() , NULL) ||
	ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	else return -1;*/


	string fileName = "output_";
	fileName.append(to_string(n));
	fileName.append(".bin");
	if (toLog) printf_s("Starting to write the file %s... ", fileName.c_str());
	auto start = chrono::high_resolution_clock::now();
	int err = fopen_s(&f, fileName.c_str(), "wb");
	if (err == 0) {
		for (int i = 0; i < n; i++) {
			fprintf_s(f, "%f ", mass[i]);
		}
		fclose(f);
	}
	auto finish = chrono::high_resolution_clock::now();
	unsigned long duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
	if (toLog) printf_s(" Done in %dms \n", duration);
	return duration;
}

unsigned long Base::doParralel(int cpuCount, int n) {
	return 0;
}

unsigned long Base::doSingle(int n) {
	return 0;
}

unsigned long Base::wrapAlgo(int cpuCount, int n) {
	printExName(cpuCount, n);

	int tStart = THR_START;

	int iter = APROXIME;
	unsigned long time = 0;
	for (int z = 0; z < iter; z++) {
		if (cpuCount == tStart) {
			time += doSingle(n);
			continue;
		}
		time += doParralel(cpuCount, n);
	}
	readData(n, false);
	doParralel(cpuCount, n);
	printf_s("Done in %d ns\n", time / iter);
	return time / iter;
}

void Base::printExName(int cpuCount, int n) {
	string text = "Started parralel Matrix[";
	text.append(to_string(n));
	text.append("] ");
	text.append(ExName);
	text.append("with ");
	text.append(to_string(cpuCount + 1));
	text.append(" threads... ");
	printf_s("%s", text.c_str());
}


void Base::doAnalise() {
	int max = MAX_CPU_COUNT;
	int iter = APROXIME;
	int minN = MIN_N;
	int maxN = MAX_N;
	int step = STEP_N;
	int tStep = THR_STEP;
	int tStart = THR_START;
	int tEnd = THR_END;
	unsigned long* RunTimes = (unsigned long*)malloc(sizeof(unsigned long) * max * 10);
	unsigned long* WriteTimes = (unsigned long*)malloc(sizeof(unsigned long) * max * 10);
	unsigned long* ReadTimes = (unsigned long*)malloc(sizeof(long) * max * 10);

	for (int i = tStart; i <= tEnd; i += tStep) {
		omp_set_num_threads(i + 1);

		for (int n = minN; n <= maxN; n += step) {
			mass = (float*)malloc(sizeof(float)*n);
			if (i == minN) {
				singleMass = (float*)malloc(sizeof(float)*n);
				long currentRunTime = doSingle(n);
			}
			long currentReadTime = readData(n, true);
			long currentRunTime = wrapAlgo(i, n);
			long currentWriteTime = writeData(n, true) / 1000;

			if (!isSame()) {
				cout << "Results was difrent!";
				_getch();
				exit(4);
			}

			int row = i;
			int col = (n / step) - 1;

			RunTimes[row * 10 + col] = currentRunTime;
			WriteTimes[row * 10 + col] = currentWriteTime;
			ReadTimes[row * 10 + col] = currentReadTime;

			cout << "=========================\n\n";
			free(mass);
		}
		cout << "\n";
	}

	saveResults(RunTimes, ReadTimes, WriteTimes);
	system("result.csv");
}
void Base::showResults(unsigned long* run, unsigned long* read, unsigned long* write) {
	int max = MAX_CPU_COUNT;
	cout << "\n\n==  RUN TIMES  =======================\n";
	cout << "length    ";
	for (int n = 1; n < 11; n++) printf_s("%10d", n * 1000);
	for (int i = 0; i < max; i++) {
		printf_s("\n%5d thrs", (i + 1));
		for (int n = 0; n < 10; n++) {
			printf_s("%10.2f", run[i * 10 + n] / 1000.0f);
		}
	}

	cout << "\n\n==  READ TIMES =======================\n";
	cout << "length    ";
	for (int n = 1; n < 11; n++) printf_s("%10d", n * 1000);
	for (int i = 0; i < max; i++) {
		printf_s("\n%5d thrs", (i + 1));
		for (int n = 0; n < 10; n++) {
			printf_s("%10.2f", read[i * 10 + n] / 1000.0f);
		}
	}

	cout << "\n\n==  WRITE TIMES  =====================\n";
	cout << "length    ";
	for (int n = 1; n < 11; n++) printf_s("%10d", n * 1000);
	for (int i = 0; i < max; i++) {
		printf_s("\n%5d thrs", (i + 1));
		for (int n = 0; n < 10; n++) {
			printf_s("%10.2f", write[i * 10 + n] / 1000.0f);
		}
	}
}
void Base::saveResults(unsigned long* res, unsigned long* write, unsigned long* read) {
	int maxCPU = MAX_CPU_COUNT;
	int step = STEP_N;
	FILE *f;
	string fileName = "result.bin";
	int err = fopen_s(&f, fileName.c_str(), "w");
	if (err == 0) {

		for (int i = 0; i < maxCPU * 10; i++) {
			fprintf_s(f, "%d\n", res[i]);
		}
		fclose(f);

	}

	FILE *csv;
	int max = MAX_CPU_COUNT;
	int tStep = THR_STEP;
	int tStart = THR_START;
	int tEnd = THR_END;
	err = fopen_s(&csv, "result.csv", "w");
	if (err == 0) {
		fprintf_s(csv, ";;\n;==  RUN; TIME;;;;;;;;;;;==  READ; TIME;;;;;;;;;;;== WRITE; TIMES\n;");
		for (int n = 1; n < 11; n++) fprintf_s(csv, "%10d;", n * step);
		fprintf_s(csv, " ; ; ;");
		for (int n = 1; n < 11; n++) fprintf_s(csv, "%10d;", n * step);
		fprintf_s(csv, " ; ; ;");
		for (int n = 1; n < 11; n++) fprintf_s(csv, "%10d;", n * step);
		for (int i = tStart; i <= tEnd; i += tStep) {
			fprintf_s(csv, "\n%d thrs;", (i + 1));
			for (int n = 0; n < 10; n++) {
				int row = i;
				fprintf_s(csv, "%d;", res[row * 10 + n]);
			}
			fprintf_s(csv, " ; ;%5d thrs;", (i + 1));
			for (int n = 0; n < 10; n++) {
				int row = i;
				fprintf_s(csv, "%d;", read[row * 10 + n]);
			}
			fprintf_s(csv, " ; ;%5d thrs;", (i + 1));
			for (int n = 0; n < 10; n++) {
				int row = i;
				fprintf_s(csv, "%d;", write[row * 10 + n]);
			}
		}
		fclose(f);
		fclose(csv);

	}
}

bool Base::isSame() {
	for (int i = 0; i < singleSize; i++) {
		if (mass[i] != singleMass[i]) return false;
	}
}

Base::~Base()
{
}
