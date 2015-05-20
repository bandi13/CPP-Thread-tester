//============================================================================
// Name        : parallelProc.cpp
// Author      : Andras Fekete
// Version     :
// Copyright   : Copyright 2015
// Description : Hello World in C, Ansi-style
//============================================================================

#define ARRSIZE 10000

int parProc(int *arr);

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)
