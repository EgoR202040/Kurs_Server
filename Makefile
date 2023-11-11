.PHONY: all clean format

CXX=g++
CXXFLAGS=-O2  -DNDEBUG -std=c++17
LDFLAGS= -lUnitTest++ -lboost_program_options -lcrypto++
PROJECT = tests
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:%.cpp=%.o)

all : $(PROJECT)

$(PROJECT) : $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@
	rm -f *.o *.orig

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS) : $(HEADERS)

format:
	astyle *.cpp *.h
	
clean:
	rm -f $(PROJECT) *.o *.orig

