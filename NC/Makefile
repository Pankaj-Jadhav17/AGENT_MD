# NC — Numerical Computation Toolbox
# Makefile for C++17 build

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCS = src/Matrix.cpp \
       src/SLE.cpp \
       src/GaussElimination.cpp \
       src/GaussJacobi.cpp \
       src/GaussSeidel.cpp \
       src/LUCrout.cpp \
       src/LUDoolittle.cpp \
       src/LUCholesky.cpp \
       src/EigenValue.cpp \
       src/Interpolation.cpp \
       src/Lagrange.cpp \
       src/LeastSquares.cpp \
       src/CurveFitting.cpp \
       main.cpp

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

TARGET = nc

all: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p output

clean:
	rm -f src/*.o *.o $(TARGET)

.PHONY: all clean dirs
