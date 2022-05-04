#include "simulation.c"
#include <stdio.h>
#include <time.h>

int main(){

    srand(time(NULL));
    struct Board one;
    struct Board two;

    clock_t st = clock();
    one = initialize_board(24, 80);
    two = initialize_board(24, 80);
    st = clock()-st;
    printf("Tiempo inicializacion: %fs\n", ((double)st)/CLOCKS_PER_SEC);

    st = clock();
    one = create_life(one, 20);
    st = clock()-st;
    printf("Tiempo Vida: %fs\n", ((double)st)/CLOCKS_PER_SEC);

    //print_board(one);

    //printf("\n");
    int i;
    for(i=0;i<300;i++){
        //st = clock();
        two = simulation_step(one);
        print_board(two);
        usleep(50000);
        printf("\n");
        one = simulation_step(two);
        print_board(one);
        usleep(50000);
        printf("\n");
        //st = clock()-st;
        //printf("Tiempo Simulacion: %fs\n", ((double)st)/CLOCKS_PER_SEC);

    }
    

    //print_board(two);


}
