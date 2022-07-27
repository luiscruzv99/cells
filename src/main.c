#include "simulation.c"
#include <stdio.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(){

    srand(time(NULL));
    struct Board one;
    struct Board two;

    // Get the terminal size to adapt the board
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);


    clock_t st = clock();
    one = initialize_board(w.ws_row-1, w.ws_col-1);
    two = initialize_board(w.ws_row-1, w.ws_col-1);
    
    one = create_life(one, 20);
    
    //print_board(one);

    //printf("\n");
    int i;
    st = clock();
    for(i=0;i<300;i++){
        two = simulation_step(one);
        print_board(two);
        usleep(50000);
        //printf("\n");
        one = simulation_step(two);
        //print_board(one);
        //usleep(50000);
        //printf("\n");
    }
    st = clock()-st;
    printf("Tiempo Simulacion: %fs\n", ((double)st)/CLOCKS_PER_SEC);
    //print_board(two);
}
