all: test2 test4 test8 test16 

#GITVERSION := $(shell git describe --abbrev=4 --dirty --always --tags)

GITVERSION= "no-version"

CXXFLAGS+= -std=gnu++11 -march=native -DGITVERSION=\"$(GITVERSION)\" -Wno-format

CXXFLAGS+= -Wno-pointer-arith -DKMERLENGTH=20 -DBIT_PER_BP=2 -O3


SRC= testOthelloKeyValue.cc

HEADERS= *.h

.PHONY: clean

test2: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@ $(SRC) -DVALUELENGTH=2

test4: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@ $(SRC) -DVALUELENGTH=4

test8: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@ $(SRC) -DVALUELENGTH=8

test16: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@ $(SRC) -DVALUELENGTH=16

clean:	
	rm test

run: test4
	./test4 2 smallkmers.txt tmp

testRatio: testRatio.cc othello.h
