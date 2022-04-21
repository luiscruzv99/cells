all: compile run

compile:
	gcc -o cells src/main.c

run:
	./cells