CXXFLAGS=-O -g -std=c++17
CFLAGS=-O -g

.PHONY: build
build : carithmetic cpparithmetic cgeometric cppgeometric

carithmetic : arithmetic.c
	$(CC) $(CFLAGS) -o $@ $<

cpparithmetic : arithmetic.cpp averager.h
	$(CXX) $(CXXFLAGS) -o $@ $<

cgeometric : geometric.c
	$(CC) $(CFLAGS) -o $@ $<

cppgeometric : geometric.cpp averager.h
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: run
run : run-carithmetic run-cpparithmetic run-cgeometric run-cppgeometric

.PHONY: run-carithmetic
run-carithmetic : carithmetic
	./carithmetic

.PHONY: run-cpparithmetic
run-cpparithmetic : cpparithmetic
	./cpparithmetic

.PHONY: run-cgeometric
run-cgeometric : cgeometric
	./cgeometric

.PHONY: run-cppgeometric
run-cppgeometric : cppgeometric
	./cppgeometric

clean:
	/bin/rm -rf *~ carithmetic cpparithmetic cgeometric cppgeometric

