#include "simulation.c"
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

int main() {

  //srand(time(NULL));
  struct Board one;
  struct Board two;

  int tam = 10;

  // Get the terminal size to adapt the board
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  clock_t st = clock();
  //one = initialize_board(w.ws_row - 1, w.ws_col - 1);
  //two = initialize_board(w.ws_row - 1, w.ws_col - 1);

  one = initialize_board(tam, tam*4);
  two = initialize_board(tam, tam*4);

  clock_t init = clock() - st;

  st = clock();
  one = create_life(one, 20);
  clock_t pop = clock() - st;

  int i;
  st = clock();
  for (i = 0; i < 1000; i++) {
    simulation_step(one, two);
    print_board(two);
    usleep(5000);
    simulation_step(two, one);
  }

  st = clock() - st;
  printf("Tiempo init: %fs\n", ((double)init) / CLOCKS_PER_SEC);
  printf("Tiempo pop: %fs\n", ((double)pop) / CLOCKS_PER_SEC);
  printf("Tiempo Simulacion: %fs\n", ((double)st) / CLOCKS_PER_SEC);
}
