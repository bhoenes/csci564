PROGRAM = bchAtlas.out
CXX = gcc
CXXFLAGS = -Wall
LDFLAGS = -latlas -lblas -lrt
OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

all:    $(PROGRAM)

.c.o :
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS)

clean:
	rm -f *.o *.out
