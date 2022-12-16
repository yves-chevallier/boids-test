EXEC = boids
CC = clang++
CFLAGS = -std=c++2a -c -Wall
LDFLAGS = -std=c++2a -o $(EXEC) -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS) sfml-widgets/lib/libsfml-widgets.a
	$(CC) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) -rf *o $(EXEC)
