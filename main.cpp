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
#ifndef NOBOOST
	#include <boost/thread.hpp>
	#define TOTALTESTS 6
#else
	#define TOTALTESTS 5
#endif

using namespace std;
#include "commonIncludes.h"

#define NUMRUNS 2

void *pthreadParProc(void *arr) { return (void *)parProc(*((int *)arr)); }
typedef union data_u {
	long tests[TOTALTESTS];
	struct {
		long serial;
		long async;
		long omp;
		long pthread;
		long thread;
#ifndef NOBOOST
		long boost;
#endif
	};
} data_ut;
string testNames[] = {
	"serial"
	,"async"
	,"omp"
	,"pthread"
	,"thread"
#ifndef NOBOOST
	,"boost"
#endif
};

data_ut runTest(int numRuns, int n) {
	data_ut ret;
	{
		auto startTime = chrono::system_clock::now();
		for(int i = 0; i < numRuns; i++) parProc(n);
		ret.serial = std::chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - startTime).count();
	}
	{
		auto startTime = chrono::system_clock::now();
		auto asyncfut = new future<bool> [numRuns];
		for(int i = 0; i < numRuns; i++) asyncfut[i] = async(launch::async, parProc, n);
		for(int i = 0; i < numRuns; i++) asyncfut[i].get();
		delete [] asyncfut;
		ret.async = std::chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - startTime).count();
	}
	{
		auto startTime = chrono::system_clock::now();
		#pragma omp parallel for
		for(int i = 0; i < numRuns; i++) parProc(n);
		ret.omp = std::chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - startTime).count();
	}
	{
		auto startTime = chrono::system_clock::now();
		pthread_t *pthreads = new pthread_t[numRuns];
		int retVal;
		for(int i = 0; i < numRuns; i++) pthread_create(&pthreads[i], NULL, pthreadParProc, (void*)&n);
		for(int i = 0; i < numRuns; i++) pthread_join(pthreads[i],(void **)&retVal);
		delete [] pthreads;
		ret.pthread = std::chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - startTime).count();
	}
	{
		auto startTime = chrono::system_clock::now();
		thread *threads = new thread[numRuns];
		for(int i = 0; i < numRuns; i++) threads[i] = thread(parProc,n);
		for(int i = 0; i < numRuns; i++) threads[i].join();
		delete [] threads;
		ret.thread = std::chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - startTime).count();
	}
#ifndef NOBOOST
	{
		auto startTime = chrono::system_clock::now();
		boost::thread *boostThreads = new boost::thread[numRuns];
		for(int i = 0; i < numRuns; i++) boostThreads[i] = boost::thread(parProc,n);
		for(int i = 0; i < numRuns; i++) boostThreads[i].join();
		delete [] boostThreads;
		ret.boost = std::chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - startTime).count();
	}
#endif

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

	cout << testNames[0];
	for(int i = 1; i < TOTALTESTS; i++) cout << "\t" << testNames[i];
	cout << endl;

	int numProc = atoi(argv[1]);
	data_ut data;
	for(int i = 0; i < TOTALTESTS; i++) data.tests[i] = 0;
	for(int j = 0; j < NUMRUNS; j++) {
		data_ut curTest = runTest(numProc,arr[j]);

		for(int i = 0; i < TOTALTESTS; i++) data.tests[i] += curTest.tests[i];
	}
	bool comma = false;
	for(int i = 0; i < TOTALTESTS; i++) { if(comma) cout << '\t'; cout << ((float)data.tests[i] / 1000.0 / NUMRUNS); comma = true; }
	cout << endl;

	cout << flush;
	delete [] arr;
	return EXIT_SUCCESS;
}
