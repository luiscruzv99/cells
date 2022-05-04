#include "types.h"

#include <stdio.h>
#include <stdlib.h>

void clrscr()
{
    system("clear");
}

struct Board initialize_board(int rows, int cols){
    struct Board b;

    b.rows = rows;
    b.cols = cols;
    b.board = (struct Cell *) malloc(rows*cols*sizeof(struct Cell));

    int i,j;

    #pragma omp parallel for
    for(i=0; i<(rows*cols); i++)
        b.board[i].alive=0;

    return b;
}

struct Board create_life(struct Board b, unsigned int quantity){
    int i;

    for(i=0; i<quantity; i++){

        // Coger celula aleatoria
        int x = rand() % b.rows;
        int y = rand() % b.cols;

        if(!b.board[x*b.cols+y].alive){
            // Generar secuencia de vida
            b.board[x*b.cols+y].life = rand();
            // Asignar a la celula y ponerla a viva
            b.board[x*b.cols+y].alive = 1;
        }else{
            i--;
        }
    }

    return b;
}
void print_board(struct Board b){
    int i,j;
  
    //printf("\e[1;1H\e[2J");
    clrscr();
    for(i=0; i<b.rows; i++){
        for(j=0; j<b.cols; j++){
            if(b.board[i*b.cols+j].alive){
                struct Cell c = b.board[i*b.cols+j];
                unsigned int shape = c.life & shape_mask;
                printf("%c", shapes[shape]);
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }

}

struct Cell mutate_cell(struct Cell c){
    // Saca el indice de mutacion de la celula (bs de 8 hacia izq)
    int mut = mutate_mask & c.life;
    mut = mut >> 8;
    // Ver si toca mutar
    unsigned int a = rand() % 32;
    if(mut > a){
        // Generar numero aleatorio de 3 bits
        unsigned int b = (rand() % 6) + 1;
        unsigned int d = rand() % 12;

        //Bit hackery
        b = b << d;
        c.life = c.life & ~b;
    }

    return c;
}

struct Board grow_cell(struct Board b, int i, int j){
    // Sacar el indice de crecimiento de la celula (bs de 13 hacia izq)
    struct Cell c;
    c = b.board[i*b.cols+j];
    
    int grow = grow_mask & c.life;
    grow = grow >> 13;
    
    // Ver si toca crecer
    unsigned int a = rand() % 128;
    if(grow > a){
        int its = 0;

        do{
            // Elegir un espacio donde expandir
            int x = (rand() % 3) - 1;
            int y = (rand() % 3) - 1;

            if((i+x)>0 && (i+x)<b.rows && (j+y)>0 && (j+y)<b.cols){
                // Mirar si se puede crecer
                if(!b.board[(i+x)*b.cols+(j+y)].alive){
                    // Copiar info de la célula
                    b.board[(i+x)*b.cols+(j+y)].alive = 1;
                    b.board[(i+x)*b.cols+(j+y)].life = c.life;
                    its = 8;
                }
            }

            its++;

        }while (its < 8);
    }

    return b;
}

struct Cell kill_cell(struct Cell c){
    // Sacar el indice de mortalidad de la celula (bs de 11 hacia izq)
    int die = die_mask & c.life;
    die = die >> 11;
    // Ver si toca morir
    unsigned int a = rand() % 128;
    if(die > a || die == 0)
        // Poner alive a 0
        c.alive = 0;
    
    return c;
}

struct Board simulation_step(struct Board from){
    // Recorre el tablero y simula cada celula, copiando el estado a otro tablero
    // Para cada celula, crecer, mutar, morir -> a su correspondiente celula en el otro tablero
    int i, j;

    //#pragma omp parallel shared(from) private(i,j)
    //#pragma omp for
    for(i=0; i<from.rows; i++)
        for(j=0; j<from.cols; j++){
            struct Cell c = from.board[i*from.cols+j];
            if(c.alive){
                from = grow_cell(from, i, j);
                from.board[i*from.cols+j] = mutate_cell(c);
                from.board[i*from.cols+j] = kill_cell(c);
            }
        }

    return from;
}


