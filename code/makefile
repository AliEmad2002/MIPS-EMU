CXX      = g++
CXXFLAGS = -g -std=c++17
INCLUDES = -I Inc
TARGET   = main.exe
SRCS     = $(wildcard Src/*.cpp)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(INCLUDES) -o $(TARGET)

clean:
	del $(TARGET)

# run "mingw32-make" to execute this make