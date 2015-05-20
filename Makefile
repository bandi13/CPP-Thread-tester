CXXFLAGS = -Wall -fmessage-length=0 --std=c++11
TARGET = main_parProc main_prime
LIBS = -lpthread -fopenmp

all:	$(TARGET)

$(OBJS): %.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main_parProc:	main.cpp parProc.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main_prime:	main.cpp prime.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

parProc.s: %.s : %.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ -S $<

debug: CXXFLAGS += -Og -pg
debug: main

clean:
	rm -f $(TARGET) parProc.s *.o
