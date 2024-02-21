CXXFLAGS=-O -g -std=c++20
CFLAGS=-O -g
LDFLAGS=-lm

.PHONY: build
build : carithmetic cpparithmetic cgeometric cppgeometric

carithmetic : arithmetic.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

cpparithmetic : arithmetic.cpp averager.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

cgeometric : geometric.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

cppgeometric : geometric.cpp averager.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

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

