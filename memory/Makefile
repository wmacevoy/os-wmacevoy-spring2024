CXXFLAGS=-O -g -std=c++17

canary.o : canary.cpp canary.h randomize.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<
testcanary.o : testcanary.cpp canary.h
	$(CXX) -c -o $@ $(CXXFLAGS) $<
testcanary : testcanary.o canary.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LDFLAGS)

good: testcanary
	./testcanary 0 10 || echo failed
	./testcanary 1 9 || echo failed

bad: testcanary
	./testcanary 0 11 && echo failed
	./testcanary -1 10 && echo failed

storage : storage.cpp canary.cpp canary.h randomize.h
	$(CXX) -o $@ $(CXXFLAGS) storage.cpp canary.cpp $(LDFLAGS)

