all: compile run

compile:
	gcc -O3 -o cells src/main.c -lm

run:
	./cells