CXXFLAGS = -Wall -fmessage-length=0 --std=c++11
OBJS =	parProc.o
LIBS = -lpthread -fopenmp

all:	main

$(OBJS): %.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main:	main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o main main.cpp $(OBJS) $(LIBS)

parProc.s: parProc.cpp parProc.h
	$(CXX) $(CXXFLAGS) -o parProc.s -S parProc.cpp

debug: CXXFLAGS += -Og -pg
debug: main

clean:
	rm -f main parProc.s $(OBJS)
