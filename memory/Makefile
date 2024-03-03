ifeq ($(OS),Windows_NT)
    SHELL := C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/bash
else
    SHELL := /bin/bash
endif

CXXFLAGS=-O -g -std=c++17

randomize.o : randomize.cpp randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testrandomize.o : testrandomize.cpp randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testrandomize : testrandomize.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

canary.o : canary.cpp canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testcanary.o : testcanary.cpp canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testcanary : testcanary.o canary.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

securedata.o : securedata.cpp securedata.h canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testsecuredata.o : testsecuredata.cpp securedata.h canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testsecuredata : testsecuredata.o securedata.o canary.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

testsecurearray.o : testsecurearray.cpp securedata.h canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testsecurearray : testsecurearray.o securedata.o canary.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

all : test

test : testrandomize testcanary testsecuredata testsecurearray
	./testrandomize && ./testcanary && ./testsecuredata && ./testsecurearray && echo "passed"

clean :
	/bin/rm -rf *.o testrandomize testcanary testsecuredata testsecurearray
