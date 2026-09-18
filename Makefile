CC = clang
CXX = clang++
CFLAGS = -Wall -DGL_SILENCE_DEPRECATION -I./Libraries/include -I/opt/homebrew/include -I/usr/local/include
CXXFLAGS = -std=c++17 -Wall -DGL_SILENCE_DEPRECATION -I./Libraries/include -I/opt/homebrew/include -I/usr/local/include
LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

OBJS = Main.o glad.o shaderClass.o VBO.o EBO.o VAO.o
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
