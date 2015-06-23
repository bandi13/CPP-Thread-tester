CXXFLAGS = -Wall -fmessage-length=0 --std=c++11 -fpermissive -O2
TARGET = main_asmProc main_prime main_nop
LIBS = -lpthread -fopenmp -lboost_system -lboost_thread

all:	$(TARGET)

$(OBJS): %.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

main_asmProc:	main.cpp asmProc.cpp commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main_prime:	main.cpp prime.cpp commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main_nop:	main.cpp nop.cpp commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

prime.s: %.s : %.cpp commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ -S $<

asmProc.s: %.s : %.cpp commonIncludes.h
	$(CXX) $(CXXFLAGS) -o $@ -S $<

debug: CXXFLAGS += -Og -pg
debug: main

clean:
	rm -f $(TARGET) *.s *.o
