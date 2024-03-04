SHELL := /bin/bash
ifeq ($(OS),Windows_NT)
#    SHELL := C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/bash
    LDFLAGS=-lbcrypt -Wl,--subsystem,console
else
# nop
endif

CXXFLAGS=-O -g -std=c++17

monitor.o : monitor.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

monitor : monitor.o
	$(CXX) -o $@ $(CXXFLAGS) $< $(LDFLAGS)

randomize.o : randomize.cpp randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testrandomize.o : testrandomize.cpp randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testrandomize : testrandomize.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

test-randomize: testrandomize monitor
	./monitor pass ./testrandomize

canary.o : canary.cpp canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testcanary.o : testcanary.cpp canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testcanary : testcanary.o canary.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

test-canary: testcanary monitor
	./monitor pass ./testcanary --ok=true && ./monitor fail ./testcanary --ok=false

securedata.o : securedata.cpp securedata.h canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testsecuredata.o : testsecuredata.cpp securedata.h canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testsecuredata : testsecuredata.o securedata.o canary.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

test-securedata : testsecuredata monitor
	./monitor pass ./testsecuredata --ok=true && ./monitor fail ./testsecuredata --ok=false

testsecurearray.o : testsecurearray.cpp securearray.h securedata.h canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<

testsecurearray : testsecurearray.o securedata.o canary.o randomize.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

test-securearray : testsecurearray monitor
	./monitor pass ./testsecurearray --ok=true && ./monitor fail ./testsecurearray --ok=false

all : test

test : test-randomize test-canary test-securedata test-securearray

clean :
	/bin/rm -rf *.o monitor testrandomize testcanary testsecuredata testsecurearray
