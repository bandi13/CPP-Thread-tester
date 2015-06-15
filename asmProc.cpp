//============================================================================
// Name        : parProc.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Do a lot of non-branching assembly instructions in the body of the for loop.
//============================================================================

#include "commonIncludes.h"
#include <cstddef>

bool parProc(int n) {
	int sum1 = 0x1000, sum2 = 1;
	n >>= 4;

	for(int i = 0; i < n; i++) {
		sum1 += sum2;
		sum2 += sum2 + 1;
		sum1 *= sum2;
		sum1 >>= 4;
		sum2 += n;
		sum1 ^= sum2;
		sum2 ^= sum2 + 1;
		sum1 *= sum2;
		sum1 >>= 4;

		sum1 += sum2;
		sum2 += sum2 + 1;
		sum1 *= sum2;
		sum1 >>= 4;
		sum2 += n;
		sum1 ^= sum2;
		sum2 ^= sum2 + 1;
		sum1 *= sum2;
		sum1 >>= 4;

		sum1 += n * sum2;
		sum1 += (n+1) * sum2;
		sum1 += (n+2) * sum2;
		sum1 += (n+3) * sum2;
		sum1 *= (n+4) * sum2;
		sum1 >>= 4;
		sum1 += n * sum2;
		sum1 += (n-1) * sum2;
		sum1 += (n-2) * sum2;
		sum1 += (n-3) * sum2;
		sum1 *= (n-4) * sum2;
		sum1 >>= 4;
		sum1 ^= n * sum2;
		sum1 ^= (n+1) * sum2;
		sum1 ^= (n+2) * sum2;
		sum1 ^= (n+3) * sum2;
		sum1 *= (n+4) * sum2;
	}
	return sum1 | sum2;
}

