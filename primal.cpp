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
	if(unlikely(arr[0] < 0)) return false;
	if(unlikely(arr[0] == 2)) return true;
	bool prime = true;
	for(int j = 3; j < arr[0] / 2; j += 2) if(unlikely(arr[0] % j == 0)) { prime = false; }
	return prime;
}

