CXX = g++

TARGET = thinning

SOURCES = thinning.cpp

OBJECTS = $(SOURCES:.cpp=.o)

INCLUDES = -I/usr/include/opencv2

LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ximgproc -lopencv_imgcodecs

CXXFLAGS = -std=c++11 -Wall $(INCLUDES)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
