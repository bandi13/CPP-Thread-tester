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

using namespace std;
#include "parProc.h"

void *pthreadParProc(void *arr) { return (void *)parProc((int *)arr); }

float runTest(int testNo, int numRuns, int *arr) {
	auto startTime = chrono::system_clock::now();
	switch(testNo) {
		case 0:
			for(int i = 0; i < numRuns; i++) parProc(arr);
			break;
		case 1: {
			auto asyncfut = new future<int> [numRuns];
			for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, arr);
			for(int i = 0; i < numRuns; i++) asyncfut[i].get();
			delete [] asyncfut;
			}
			break;
		case 2: {
			#pragma omp parallel for
			for(int i = 0; i < numRuns; i++) parProc(arr);
			}
			break;
		case 3: {
			pthread_t *pthreads = new pthread_t[numRuns];
			for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)arr);
			int retVal;
			for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
			delete [] pthreads;
			}
			break;
		case 4: {
			thread *threads = new thread[numRuns];
			for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,arr);
			for(int i = 0; i < numRuns; i++) threads[i].join();
			delete [] threads;
			}
			break;
		default: return 0;
	}
	auto endTime = chrono:: system_clock::now();

	chrono::duration<double> diff = endTime - startTime;
	return diff.count() * 1000;
}

float runTest2(int testNo, int numRuns, int *arr) {
	auto startTime = chrono::system_clock::now();
	auto endTime = chrono::system_clock::now();
	switch(testNo) {
		case 0:
			startTime = chrono::system_clock::now();
			for(int i = 0; i < numRuns; i++) parProc(arr);
			endTime = chrono:: system_clock::now();
			break;
		case 1: {
			auto asyncfut = new future<int> [numRuns];
			startTime = chrono::system_clock::now();
			for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, arr);
			for(int i = 0; i < numRuns; i++) asyncfut[i].get();
			endTime = chrono:: system_clock::now();
			delete [] asyncfut;
			}
			break;
		case 2: {
			startTime = chrono::system_clock::now();
			#pragma omp parallel for
			for(int i = 0; i < numRuns; i++) parProc(arr);
			endTime = chrono:: system_clock::now();
			}
			break;
		case 3: {
			pthread_t *pthreads = new pthread_t[numRuns];
			startTime = chrono::system_clock::now();
			for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)arr);
			int retVal;
			for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
			endTime = chrono:: system_clock::now();
			delete [] pthreads;
			}
			break;
		case 4: {
			thread *threads = new thread[numRuns];
			startTime = chrono::system_clock::now();
			for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,arr);
			for(int i = 0; i < numRuns; i++) threads[i].join();
			endTime = chrono:: system_clock::now();
			delete [] threads;
			}
			break;
		default: return 0;
	}

	chrono::duration<double> diff = endTime - startTime;
	return diff.count() * 1000;
}

#define NUMRUNS 20
int main(int argc, char *argv[]) {
	if(argc < 2) {
		cerr << "Usage : " << argv[0] << " <procCount>" << endl;
		return EXIT_FAILURE;
	}

	auto arr = new int[ARRSIZE];
	for(int i = 0; i < ARRSIZE; i++) arr[i] = rand();

	int numProc = atoi(argv[1]);
	float data[5];
	for(int i = 0; i < 5; i++) data[i] = 0.0;
	for(int j = 0; j < NUMRUNS; j++) {
		for(int i = 0; i < 5; i++) { data[i] += runTest(i,numProc,arr); }
	}
	cout << "serial\tasync\tOpenMP\tpthread\tthread\tserial2\tasync2\tOpenMP2\tpthread2\tthread2" << endl;
	for(int i = 0; i < 5; i++) cout << (data[i] / NUMRUNS) << '\t';

	for(int i = 0; i < 5; i++) data[i] = 0.0;
	for(int j = 0; j < NUMRUNS; j++) {
		for(int i = 0; i < 5; i++) { data[i] += runTest2(i,numProc,arr); }
	}
	for(int i = 0; i < 5; i++) cout << (data[i] / NUMRUNS) << '\t';
	cout << endl;

	delete [] arr;
	return EXIT_SUCCESS;
}
