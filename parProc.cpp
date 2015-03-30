//============================================================================
// Name        : parallelProc.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <future>
#include <iostream>
#include <chrono>

using namespace std;

#include "parProc.h"

int parProc(int *arr) {
	int sum1 = 0, sum2 = 0;

	for(int i = 4; i < ARRSIZE - 4; i++) {
		sum1 += arr[i];
		sum1 += arr[i+1];
		sum1 += arr[i+2];
		sum1 += arr[i+3];
		sum1 *= arr[i+4];
		sum1 >>= 4;
		sum1 += arr[i];
		sum1 += arr[i-1];
		sum1 += arr[i-2];
		sum1 += arr[i-3];
		sum1 *= arr[i-4];
		sum1 >>= 4;
		sum1 ^= arr[i];
		sum1 ^= arr[i+1];
		sum1 ^= arr[i+2];
		sum1 ^= arr[i+3];
		sum1 *= arr[i+4];

		sum2 += arr[i];
		sum2 += arr[i+1];
		sum2 += arr[i+2];
		sum2 += arr[i+3];
		sum2 *= arr[i+4];
		sum2 >>= 4;
		sum2 += arr[i];
		sum2 += arr[i-1];
		sum2 += arr[i-2];
		sum2 += arr[i-3];
		sum2 *= arr[i-4];
		sum2 >>= 4;
		sum2 ^= arr[i];
		sum2 ^= arr[i+1];
		sum2 ^= arr[i+2];
		sum2 ^= arr[i+3];
		sum2 *= arr[i+4];

		sum1 += arr[i] * sum2;
		sum1 += arr[i+1] * sum2;
		sum1 += arr[i+2] * sum2;
		sum1 += arr[i+3] * sum2;
		sum1 *= arr[i+4] * sum2;
		sum1 >>= 4;
		sum1 += arr[i] * sum2;
		sum1 += arr[i-1] * sum2;
		sum1 += arr[i-2] * sum2;
		sum1 += arr[i-3] * sum2;
		sum1 *= arr[i-4] * sum2;
		sum1 >>= 4;
		sum1 ^= arr[i] * sum2;
		sum1 ^= arr[i+1] * sum2;
		sum1 ^= arr[i+2] * sum2;
		sum1 ^= arr[i+3] * sum2;
		sum1 *= arr[i+4] * sum2;
	}
	return sum1 | sum2;
}

