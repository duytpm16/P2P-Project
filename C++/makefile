CXX=g++
CXXFLAGS = -std=c++11 -O3 -Wall -g


LIBS= -pthread -lboost_program_options -lboost_thread

CXX_SOURCES= bnode.cpp bgraph.cpp node.cpp graph.cpp route.cpp args.cpp p2p.cpp
CXX_OBJECTS= $(CXX_SOURCES:.cpp=.o)
C_OBJECTS= $(C_SOURCES:.c=.o)
OBJS= $(CXX_OBJECTS) $(C_OBJECTS)

#targets
all: p2p

p2p: $(CXX_OBJECTS) $(C_OBJECTS)
	$(CXX) $^ $(CXXFLAGS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

.PHONY: clean
clean:
	rm -f $(OBJS)
