
.DEFAULT_GOAL := sfml-app

main.o:  main.cpp
	g++ -c main.cpp

sfml-app: main.o
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lGL
