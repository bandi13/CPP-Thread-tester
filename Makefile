CXXFLAGS = -Wall -fmessage-length=0 --std=c++11
TARGET = main_asmProc main_prime main_prime-nohints
LIBS = -lpthread -fopenmp -lboost_system -lboost_thread

all:	$(TARGET)

$(OBJS): %.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main_asmProc:	main.cpp asmProc.o commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main_prime:	main.cpp prime.o commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main_prime-nohints:	main.cpp prime-nohints.o commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

asmProc.s: %.s : %.cpp commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ -S $<

debug: CXXFLAGS += -Og -pg
debug: main

clean:
	rm -f $(TARGET) asmProc.s *.o
