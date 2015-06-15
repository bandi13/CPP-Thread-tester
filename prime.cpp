//============================================================================
// Name        : primal.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Implementation of John Weiss' original parallel procedure
//============================================================================

#include "commonIncludes.h"
#include <cstddef>

bool parProc(int n) {
	if(n <= 2) return NULL;
	bool prime = true;
	for(int j = 3; j < n / 2; j += 2) if(n % j == 0) { prime = false; }
	return prime;
}

