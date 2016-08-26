all: testRatio

GITVERSION := $(shell git describe --abbrev=4 --dirty --always --tags)

CXXFLAGS+= -std=gnu++11 -march=native -DGITVERSION=\"$(GITVERSION)\" -Wno-format

CXXFLAGS+= -Wno-pointer-arith -g


SRC= testOthelloKeyValue.cc

HEADERS= *.h

.PHONY: clean

test: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@2 $(SRC) -DVALUELENGTH=2
	g++ $(CXXFLAGS) -o $@4 $(SRC) -DVALUELENGTH=4
	g++ $(CXXFLAGS) -o $@8 $(SRC) -DVALUELENGTH=8
	g++ $(CXXFLAGS) -o $@16 $(SRC) -DVALUELENGTH=16

clean:	
	rm test

run: test
	./test4 2 /home/xinan/othello/1M.H100.list.txt tmp

testRatio: testRatio.cc othello.h
