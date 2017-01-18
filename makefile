INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

COMPILERFLAGS = -Wall
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lglut -lGL -lGLU -lm 

all: busdriver mapmaker

busdriver : main.o globals.o image.o object.o map.o
	$(CC) $(CFLAGS) main.o globals.o image.o object.o map.o $(LIBDIR) $(LIBRARIES) -o busdriver


main.o: main.cpp
	$(CC) -c main.cpp 

globals.o: globals.cpp
	$(CC) -c globals.cpp 

image.o: image.cpp
	$(CC) -c image.cpp 

object.o: object.cpp
	$(CC) -c object.cpp 

map.o: map.cpp
	$(CC) -c map.cpp

 

mapmaker : mapmaker.cpp
	$(CC) mapmaker.cpp -o mapmaker


clean:
	rm -rf *.o busdriver mapmaker


