CXX       = g++
CXXFLAGS  = -Wall -O3 -std=c++11 

PROGRAM   = mandelbrot
SRCS      = mandelbrot.cpp
OBJS      = $(SRCS:.cpp=.o)
RM        = rm -f

.SUFFIXES: .cpp .o

.cpp.o: 
	$(CXX) $(CXXFLAGS) -c $<

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(SRCS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROGRAM)
