
CXX = g++

CXXFLAGS = -std=c++14 -Wall -Wextra -O3

OBJ = main.o MemoryPool.o

TARGET = main.exe

.PHONY: all clean run valgrind

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-error-list=yes --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
