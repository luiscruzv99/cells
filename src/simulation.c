#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void clrscr() { system("clear"); }

void initialize()
{

  // To initially generate the
  // table algorithmically
  BitsSetTable256[0] = 0;

  for (int i = 0; i < 256; i++)
  {
    BitsSetTable256[i] = (i & 1) +
                         BitsSetTable256[i / 2];
  }
}

int countSetBits(int n)
{
  return (BitsSetTable256[n & 0xff] +
          BitsSetTable256[(n >> 8) & 0xff] +
          BitsSetTable256[(n >> 16) & 0xff] +
          BitsSetTable256[n >> 24]);
}

struct Board initialize_board(int rows, int cols)
{
  struct Board b;

  b.rows = rows;
  b.cols = cols;
  b.board = (struct Cell *)malloc(rows * cols * sizeof(struct Cell));

  return b;
}

struct Board create_life(struct Board b, unsigned int quantity)
{

  int i, j, k, lim;

  float mins[b.rows*b.cols];

  float min, max, dist;

  lim = fmax(5, b.rows * b.cols * 0.005);

  int hotspots[lim * 2];

  #pragma omp parallel shared(hotspots, lim, b) private(k)
  #pragma omp for
  for (k = 0; k < lim; k++)
  {
    hotspots[k * 2] = rand() % b.rows;
    hotspots[k * 2 + 1] = rand() % b.cols;
  }

  max = 0.0;

  #pragma omp parallel shared(max, hotspots, lim, b, mins) private(i, j, k, min, dist)
  #pragma omp for
  for (i = 0; i < b.rows; i++)
  {
    for (j = 0; j < b.cols; j++)
    {

      min = 100.0;

      for (k = 0; k < lim * 2; k += 2)
      {
        dist = sqrt((i - hotspots[k]) * (i - hotspots[k])) + ((j - hotspots[k + 1]) * (j - hotspots[k + 1]));
        if (min > dist)
          min = dist;
      }

      if(min > max) max = min;
      
      mins[i*b.cols+j] = min;
    }
  }

  //#pragma omp parallel shared(b, mins, max) private(i)
  for(i=0; i < b.rows*b.cols; i++){
    b.board[i].foodCode =  (int) ceil((mins[i] / max) * 7.0);
    b.board[i].foodQuantity = rand() % 16;
    b.board[i].alive = 0;
    b.board[i].life = malloc(sizeof(struct Bacteria));
  }

  int x, y, pos;
  int tries = 5;

  do
  {

    x = rand() % b.rows;
    y = rand() % b.cols;

    pos = x * b.cols + y;

    if (!b.board[pos].alive)
    {

      // Esto debería funcionar
      char *base = (char *)b.board[pos].life;
      int *g = (int *)base + 0;
      *g = rand();
      b.board[pos].life->energy = b.board[pos].life->maxEnergy;

      b.board[pos].alive = 1;

      tries = 5;
      quantity--;
    }
    else
      tries--;
  } while (quantity > 0 && tries > 0);

  //   for(i=0; i<b.rows; i++){
  //   for(j=0; j<b.cols; j++)
  //     printf("[%d]", b.board[i*b.cols+j].foodCode);
  //   printf("\n");
  // }
  // printf("\n");


  return b;
}

void print_board(struct Board b)
{

  int i, j;

  clrscr();

  for (i = 0; i < b.rows; i++)
  {
    for (j = 0; j < b.cols; j++)
    {
      struct Cell c = b.board[i * b.cols + j];
      if (c.alive)
      {
        
        unsigned int shape = c.life->shape;
        printf( "%s%s%c%s", colors[c.life->color], bg_colors[c.foodCode], shapes[shape], colors[7]);
        
      }
      else
      {
        printf("%s %s",bg_colors[b.board[i * b.cols + j].foodCode], bg_colors[7]);
      }
      // printf("[%d]", c.foodCode);
    }
    printf("\n");
  }
}

struct Cell mutate_cell(struct Cell c)
{
  struct Bacteria *b = c.life;

  int prob = rand() % 512;

  if (prob < b->mutation)
  {
    int mut = rand() % 6 + 2;
    int offset = rand() % 26;

    // Esto debería funcionar
    int *base = (int *)b;
    int code = *base;
    code = code | (mut << offset);

    *base = code;

    b->energy--;
  }
}

struct Cell grow_cell(struct Board from, int i, int j)
{
  struct Cell c;

  int pos = i * from.cols + j;

  if (!from.board[pos].alive)
  {
    // Elegimos vecino
    //  Si vivo y se puede reproducir, reproducir
    //  Sino escoger otro vecino
  }
  return c;
}

struct Cell kill_cell(struct Cell c)
{
  int prob = rand() % 256;

  if (prob < c.life->death){
    c.life->energy--;
    if( c.life->energy > 1)
      c.alive = 0;

    // printf("Muerto");

    // int foodType = c.life->acceptFood;
    // c.foodCode = (foodType + c.foodCode) / 2;
    // c.foodQuantity += c.life->maxEnergy;
  }

  return c;
}

struct Cell eat_cell(struct Cell c)
{

  int food = c.foodCode & c.life->acceptFood;

  if (food > 0 && c.foodQuantity > 0)
  {
    c.foodQuantity--;
    c.life->energy += countSetBits(food);
  }

  return c;
}

void simulation_step(struct Board from, struct Board to)
{

  int i, j, pos;


  #pragma omp parallel private(i,j,pos) shared(from, to)
  #pragma omp for
  for (i = 0; i < from.rows; i++)
    for (j = 0; j < from.cols; j++)
    {

      pos = i * from.cols + j;

      if (from.board[pos].alive)
      {
        to.board[pos] = mutate_cell(from.board[pos]);
        to.board[pos] = eat_cell(from.board[pos]);
        to.board[pos] = kill_cell(from.board[pos]);
      }
      else
      {
        to.board[pos] = from.board[pos];
        // to.board[pos] = grow_cell(from, i, j);
      }
    }

  // return to;
}
