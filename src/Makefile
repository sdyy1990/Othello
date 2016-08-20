all: test

CXXFLAGS+= -std=gnu++11 -march=native

SRC= *.cc

HEADERS= *.h

.PHONY: clean

test: $(SRC) $(HEADERS)
	g++ $(CXXFLAGS) -o $@ $(SRC)

clean:	
	rm test


