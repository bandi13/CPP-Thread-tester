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
typedef union {
	int tests[TOTALTESTS];
	struct {
		int serial;
		int async;
		int omp;
		int pthread;
		int thread;
		int boost;
	};
} data_t;

data_t runTest(int numRuns, int n) {
	data_t ret;
	{
		auto startTime = chrono::system_clock::now();
		for(int i = 0; i < numRuns; i++) parProc(n);
		chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
		ret.serial = diff.count();
	}
	{
		auto startTime = chrono::system_clock::now();
		auto asyncfut = new future<bool> [numRuns];
		for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, n);
		for(int i = 0; i < numRuns; i++) asyncfut[i].get();
		delete [] asyncfut;
		chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
		ret.async = diff.count();
	}
	{
		auto startTime = chrono::system_clock::now();
		#pragma omp parallel for
		for(int i = 0; i < numRuns; i++) parProc(n);
		chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
		ret.omp = diff.count();
	}
	{
		auto startTime = chrono::system_clock::now();
		pthread_t *pthreads = new pthread_t[numRuns];
		int retVal;
		for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)n);
		for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
		delete [] pthreads;
		chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
		ret.pthread = diff.count();
	}
	{
		auto startTime = chrono::system_clock::now();
		thread *threads = new thread[numRuns];
		for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,n);
		for(int i = 0; i < numRuns; i++) threads[i].join();
		delete [] threads;
		chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
		ret.thread = diff.count();
	}
	{
		auto startTime = chrono::system_clock::now();
		boost::thread *boostThreads = new boost::thread[numRuns];
		for(int i = 0; i < numRuns; i++) boostThreads[i] = boost::thread(parProc,n);
		for(int i = 0; i < numRuns; i++) boostThreads[i].join();
		delete [] boostThreads;
		chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
		ret.boost = diff.count();
	}

	return ret;
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		cerr << "Usage : " << argv[0] << " <procCount>" << endl;
		return EXIT_FAILURE;
	}

	auto arr = new int[NUMRUNS];
	for(int i = 0; i < NUMRUNS; i++) arr[i] = 2 * i + 1000001;

	cout << "numProc\tserial\tasync\tOpenMP\tpthread\tthread\tboostThread" << endl;

	int numProcIn = atoi(argv[1]);
	for(int numProc = 2; numProc < numProcIn; numProc++) {
	data_t data;
	for(int i = 0; i < TOTALTESTS; i++) data.tests[i] = 0.0;
	for(int j = 0; j < NUMRUNS; j++) {
//		data_t curTest = runTest(numProc,arr[j]);
		int numRuns = numProc;
		int n = arr[j];
		data_t curTest;
		{
			auto startTime = chrono::system_clock::now();
			for(int i = 0; i < numRuns; i++) parProc(n);
			chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
			curTest.serial = diff.count();
		}
		{
			auto startTime = chrono::system_clock::now();
			auto asyncfut = new future<bool> [numRuns];
			for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, n);
			for(int i = 0; i < numRuns; i++) asyncfut[i].get();
			delete [] asyncfut;
			chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
			curTest.async = diff.count();
		}
		{
			auto startTime = chrono::system_clock::now();
			#pragma omp parallel for
			for(int i = 0; i < numRuns; i++) parProc(n);
			chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
			curTest.omp = diff.count();
		}
		{
			auto startTime = chrono::system_clock::now();
			pthread_t *pthreads = new pthread_t[numRuns];
			int retVal;
			for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)n);
			for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
			delete [] pthreads;
			chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
			curTest.pthread = diff.count();
		}
		{
			auto startTime = chrono::system_clock::now();
			thread *threads = new thread[numRuns];
			for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,n);
			for(int i = 0; i < numRuns; i++) threads[i].join();
			delete [] threads;
			chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
			curTest.thread = diff.count();
		}
		{
			auto startTime = chrono::system_clock::now();
			boost::thread *boostThreads = new boost::thread[numRuns];
			for(int i = 0; i < numRuns; i++) boostThreads[i] = boost::thread(parProc,n);
			for(int i = 0; i < numRuns; i++) boostThreads[i].join();
			delete [] boostThreads;
			chrono::duration<double, std::micro> diff = chrono::system_clock::now() - startTime;
			curTest.boost = diff.count();
		}

		for(int i = 0; i < TOTALTESTS; i++) data.tests[i] += curTest.tests[i];
	}
	cout << numProc;
	for(int i = 0; i < TOTALTESTS; i++) cout << '\t' << ((float)data.tests[i] / 1000.0 / NUMRUNS);
	cout << endl;
	}

	cout << flush;
	delete [] arr;
	return EXIT_SUCCESS;
}
