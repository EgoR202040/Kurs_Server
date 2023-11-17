.PHONY: all clean format

CXX=g++
CXXFLAGS=-O2  -DNDEBUG -std=c++17
LDFLAGS= -lUnitTest++ -lboost_program_options -lcrypto++
PROJECT = u_test
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
run:
	./$(PROJECT)
clean:
	rm -f $(PROJECT) *.o *.orig

