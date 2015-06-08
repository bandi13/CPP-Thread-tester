//============================================================================
// Name        : primal.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Implementation of John Weiss' original parallel procedure
//============================================================================

#include "commonIncludes.h"
#include <cstddef>

int *parProc(int *arr) {
	arr[0] = 32*ARRSIZE;
	if(arr[0] <= 2) return NULL;
	bool prime = true;
	UNUSED(prime);
	for(int j = 3; j < arr[0] / 2; j += 2) if(arr[0] % j == 0) { prime = false; }
	return NULL;
}

