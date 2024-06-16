
CXX=clang++
CXXFLAGS= -O3 -std=c++11 

kdtree: main.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

2dRectGrid: 2dRectGrid.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

clean:
	rm kdtree 2dRectGrid
