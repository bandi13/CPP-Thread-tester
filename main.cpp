//============================================================================
// Name        : parallelProc.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <boost/thread.hpp>

using namespace std;
#include "commonIncludes.h"

#define NUMRUNS 2
#define TOTALTESTS 6

void *pthreadParProc(void *arr) { return (void *)parProc((int)arr); }
typedef struct data_s {
	std::chrono::duration<double, std::micro> serial;
	std::chrono::duration<double, std::micro> async;
	std::chrono::duration<double, std::micro> omp;
	std::chrono::duration<double, std::micro> pthread;
	std::chrono::duration<double, std::micro> thread;
	std::chrono::duration<double, std::micro> boost;
} data_st;
typedef union data_u {
	std::chrono::duration<double, std::micro> tests[TOTALTESTS];
	data_st datas;
} data_ut;

data_ut runTest(int numRuns, int n) {
	data_ut ret;
	{
		auto startTime = chrono::system_clock::now();
		for(int i = 0; i < numRuns; i++) parProc(n);
		ret.datas.serial = chrono::system_clock::now() - startTime;
	}
	{
		auto startTime = chrono::system_clock::now();
		auto asyncfut = new future<bool> [numRuns];
		for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, n);
		for(int i = 0; i < numRuns; i++) asyncfut[i].get();
		delete [] asyncfut;
		ret.datas.async = chrono::system_clock::now() - startTime;
	}
	{
		auto startTime = chrono::system_clock::now();
		#pragma omp parallel for
		for(int i = 0; i < numRuns; i++) parProc(n);
		ret.datas.omp = chrono::system_clock::now() - startTime;
	}
	{
		auto startTime = chrono::system_clock::now();
		pthread_t *pthreads = new pthread_t[numRuns];
		int retVal;
		for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)n);
		for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
		delete [] pthreads;
		ret.datas.pthread = chrono::system_clock::now() - startTime;
	}
	{
		auto startTime = chrono::system_clock::now();
		thread *threads = new thread[numRuns];
		for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,n);
		for(int i = 0; i < numRuns; i++) threads[i].join();
		delete [] threads;
		ret.datas.thread = chrono::system_clock::now() - startTime;
	}
	{
		auto startTime = chrono::system_clock::now();
		boost::thread *boostThreads = new boost::thread[numRuns];
		for(int i = 0; i < numRuns; i++) boostThreads[i] = boost::thread(parProc,n);
		for(int i = 0; i < numRuns; i++) boostThreads[i].join();
		delete [] boostThreads;
		ret.datas.boost = chrono::system_clock::now() - startTime;
	}

	return ret;
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		cerr << "Usage : " << argv[0] << " <procCount> [difficulty]" << endl;
		cerr << "difficulty = 1 to 10" << endl;
		return EXIT_FAILURE;
	}

	uint8_t difficulty = 9;
	if(argc == 3) {
		difficulty = atoi(argv[2]);
		if((difficulty < 1) || (difficulty > 10)) {
			cerr << "Difficulty should be between 1 and 10" << endl;
			return EXIT_FAILURE;
		}
	}

	uint64_t primeNum = 1;
	for(int i = 0; i < difficulty; i++) primeNum = 10*primeNum;
	primeNum++; // make it odd

	auto arr = new int[NUMRUNS];
	for(int i = 0; i < NUMRUNS; i++) arr[i] = 2 * i + primeNum;

	cout << "numProc\tserial\tasync\tOpenMP\tpthread\tthread\tboostThread" << endl;

	int numProc = atoi(argv[1]);
	data_ut data;
	for(int i = 0; i < TOTALTESTS; i++) data.tests[i] = std::chrono::duration<double, std::micro>::zero();
	for(int j = 0; j < NUMRUNS; j++) {
		data_ut curTest = runTest(numProc,arr[j]);

		for(int i = 0; i < TOTALTESTS; i++) data.tests[i] += curTest.tests[i];
	}
	cout << numProc;
	for(int i = 0; i < TOTALTESTS; i++) cout << '\t' << ((float)std::chrono::duration_cast<std::chrono::microseconds>(data.tests[i]).count() / 1000.0 / NUMRUNS);
	cout << endl;

	cout << flush;
	delete [] arr;
	return EXIT_SUCCESS;
}
