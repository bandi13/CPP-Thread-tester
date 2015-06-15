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

void *pthreadParProc(void *arr) { return (void *)parProc((int)arr); }

float runTest(int testNo, int numRuns, int n) {
	auto startTime = chrono::system_clock::now();
	switch(testNo) {
		case 0:
			for(int i = 0; i < numRuns; i++) parProc(n);
			break;
		case 1: {
			auto asyncfut = new future<bool> [numRuns];
			for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, n);
			for(int i = 0; i < numRuns; i++) asyncfut[i].get();
			delete [] asyncfut;
			}
			break;
		case 2: {
			#pragma omp parallel for
			for(int i = 0; i < numRuns; i++) parProc(n);
			}
			break;
		case 3: {
			pthread_t *pthreads = new pthread_t[numRuns];
			int retVal;
			for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)n);
			for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
			delete [] pthreads;
			}
			break;
		case 4: {
			thread *threads = new thread[numRuns];
			for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,n);
			for(int i = 0; i < numRuns; i++) threads[i].join();
			delete [] threads;
			}
			break;
		case 5: {
			boost::thread *boostThreads = new boost::thread[numRuns];
			for(int i = 0; i < numRuns; i++) boostThreads[i] = boost::thread(parProc,n);
			for(int i = 0; i < numRuns; i++) boostThreads[i].join();
			delete [] boostThreads;
			}
			break;
		default: return 0;
	}
	auto endTime = chrono:: system_clock::now();

	chrono::duration<double> diff = endTime - startTime;
	return diff.count() * 1000;
}

#define NUMRUNS 10
#define TOTALTESTS 6
int main(int argc, char *argv[]) {
	if(argc < 2) {
		cerr << "Usage : " << argv[0] << " <procCount>" << endl;
		return EXIT_FAILURE;
	}

	auto arr = new int[TOTALTESTS];
	for(int i = 0; i < TOTALTESTS; i++) arr[i] = 2 * i + 10000001;

	int numProc = atoi(argv[1]);
	float data[TOTALTESTS];
	for(int i = 0; i < TOTALTESTS; i++) data[i] = 0.0;
	for(int j = 0; j < NUMRUNS; j++) {
		for(int i = 0; i < TOTALTESTS; i++) { data[i] += runTest(i,numProc,arr[i]); }
	}
	cout << "serial\tasync\tOpenMP\tpthread\tthread\tboostThread" << endl;
	for(int i = 0; i < TOTALTESTS; i++) cout << (data[i] / NUMRUNS) << '\t';
	cout << endl;

	delete [] arr;
	return EXIT_SUCCESS;
}
