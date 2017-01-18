INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

COMPILERFLAGS = -Wall
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lglut -lGL -lGLU -lm 

all: busdriver mapmaker

busdriver : busdriver.cpp
	$(CC) $(CFLAGS) busdriver.cpp $(LIBDIR) $(LIBRARIES) -o busdriver

mapmaker : mapmaker.cpp
	$(CC) mapmaker.cpp -o mapmaker


