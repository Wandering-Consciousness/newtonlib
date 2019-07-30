VERSION=0.91
ROOT=$(PWD)
CEXECUTABLE=example_c
EXECUTABLE=example
LIB=Attract
DYNAMICLIB=lib$(LIB).so
STATICLIB=lib$(LIB).a

CC=gcc
CXX=g++
PROD=-O3
DBG=-ggdb3
CFLAGS= -I$(ROOT) $(DBG) -m64 -Wall -Wextra
CXXFLAGS= -I$(ROOT) $(DBG) -m64 -Wall -Wextra -fPIC -std=c++11
LDFLAGS= -m64
LIBS=-L$(ROOT) -l$(LIB)

SOURCES= geometry.cpp geodesic.cpp triangulation.cpp probability.cpp mathutil.cpp scp.cpp spot.cpp kdTreeSingleThread.cpp kdTree.cpp attractors.cpp export.cpp

DEPS= $(GEOMETRY)/geometry.hpp $(GEOMETRY)/geodesic.hpp $(DELAUNATOR)/triangulation.hpp $(DELAUNATOR)/delaunator.hpp $(ATTRACTORS)/precalculated.hpp $(ATTRACTORS)/probability.hpp $(ATTRACTORS)/mathutil.hpp $(MINIBALL)/scp.hpp $(ATTRACTORS)/spot.hpp $(KBDTREE)/kdTreeSingleThread.hpp $(KBDTREE)/kdTree.hpp $(ATTRACTORS)/attractors.hpp $(ATTRACTORS)/export.hpp


OBJECTS=$(SOURCES:.cpp=.o)

all: $(STATICLIB) $(DYNAMICLIB) $(EXECUTABLE) $(CEXECUTABLE)

$(CEXECUTABLE).o: $(CEXECUTABLE).c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp 
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(STATICLIB): $(OBJECTS)
	ar ruU $@.$(VERSION) $^
	ranlib $@.$(VERSION)
	ln -s $(STATICLIB).$(VERSION) $(STATICLIB) 

$(DYNAMICLIB): $(OBJECTS)
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$(DYNAMICLIB) -o $@.$(VERSION) $(OBJECTS)
	ln -s $(DYNAMICLIB).$(VERSION) $(DYNAMICLIB) 

$(CEXECUTABLE): $(CEXECUTABLE).o
	$(CC) $(CEXECUTABLE).o $(LDFLAGS) $(LIBS) -o $@

$(EXECUTABLE): $(EXECUTABLE).o
	$(CXX) $(EXECUTABLE).o $(LDFLAGS) $(LIBS) -o $@

.PHONY: clean

clean:
	rm -rf *.o $(EXECUTABLE) $(DYNAMICLIB) $(STATICLIB) $(DYNAMICLIB).$(VERSION) $(STATICLIB).$(VERSION)

