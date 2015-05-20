//============================================================================
// Name        : parProc.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Do a lot of non-branching assembly instructions in the body of the for loop.
//============================================================================

#include "commonIncludes.h"
#include <cstddef>

int *parProc(int *arr) {
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
	return NULL;
}

