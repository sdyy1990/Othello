all: test

GITVERSION := $(shell git describe --abbrev=4 --dirty --always --tags)

CXXFLAGS+= -std=gnu++11 -march=native -DGITVERSION=\"$(GITVERSION)\" -Wno-format

CXXFLAGS+= -Wno-pointer-arith -g

SRC= testOthelloKeyValue.cc

HEADERS= *.h

.PHONY: clean

test: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@ $(SRC)

clean:	
	rm test

run: test
	./test 2 /home/xinan/othello/1M.H100.list.txt tmp
