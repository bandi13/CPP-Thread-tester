CXXFLAGS = -Wall -fmessage-length=0 --std=c++11
OBJS =	parProc.o
LIBS = -lpthread -fopenmp

.o:
	$(CXX) $(CXXFLAGS) -o $< $@
main:	main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o main main.cpp $(OBJS) $(LIBS)

parProc.s: parProc.cpp
	$(CXX) $(CXXFLAGS) -o parProc.s -S parProc.cpp

debug: CXXFLAGS += -Og -pg
debug: main

all:	main

clean:
	rm -f main parProc.s $(OBJS)
