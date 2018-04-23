CXX       = g++
CXXFLAGS  = -Wall -std=c++11 -fopenmp

PROGRAM   = firesim
SRCS      = firesim.cpp Random.cpp Forest.cpp
OBJS      = $(SRCS:.cpp=.o)

.SUFFIXES: .cpp .o

.cpp.o: 
	$(CXX) $(CXXFLAGS) -c $<

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(SRCS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
