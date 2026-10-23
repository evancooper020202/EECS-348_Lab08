CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = calculator
OBJS = main.cpp Calculator.cpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) *.o
