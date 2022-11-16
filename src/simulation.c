#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void clrscr() { system("clear"); }

void initialize()
{}

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

  float mins[b.rows * b.cols];

  float min, max, dist;

  lim = fmax(5, b.rows * b.cols * 0.005);

  int hotspots[lim * 2];

  for (k = 0; k < lim; k++)
  {
    hotspots[k * 2] = rand() % b.rows;
    hotspots[k * 2 + 1] = rand() % b.cols;
  }

  max = 0.0;

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

      if (min > max)
        max = min;

      mins[i * b.cols + j] = min;
    }
  }

  for (i = 0; i < b.rows * b.cols; i++)
  {
    b.board[i].foodCode = (int)ceil((mins[i] / max) * 7.0);
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
        printf("%s%s%c%s", colors[c.life->color],bg_colors[c.life->acceptFood], shapes[shape],colors[7]);
      }
      else
      {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void mutate_cell(struct Cell *c)
{
  struct Bacteria *b = c->life;

  int prob = rand() % 4096;

  if (b->mutation > prob)
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

void grow_cell(struct Board from, struct Cell *into, int i, int j)
{

  int pos = i * from.cols + j;

  memcpy(into, &from.board[pos], sizeof(struct Cell));
  into->life->energy = 0;

  // Mirar sus vecinos (N,S,E,W)
  int u, v;

  for (u = -1; u < 2; u++)
    if ((i + u) > 0 && (i + u) < from.rows)
      for (v = -1; v < 2; v++)
        if ((j + v) > 0 && (j + v) < from.cols)
        {
          int tmp = (i + u) * from.cols + (j + v);
          // Escoger el que tenga mas energia
          if (into->life->energy < from.board[tmp].life->energy)
          {
            int rep = rand() % 512;
            if (from.board[tmp].life->reproduction > rep && from.board[tmp].foodQuantity > 0)
            {
              memcpy(into->life, from.board[tmp].life, sizeof(struct Bacteria));
              into->alive = 1;
              from.board->life->energy -= from.board->life->reproduction;
              into->life->energy -= into->life->reproduction;
            }
          }
        }
}

void kill_cell(struct Cell *from, struct Cell *into)
{
  int prob = rand() % 256;

  if (from->life->death+1 > prob)
  {
    from->life->energy--;
    if (from->life->energy > 1)
      from->alive = 0;

    int foodType = from->life->acceptFood;
    from->foodCode = (foodType + from->foodCode) / 2;
    //from->foodQuantity ++;
  }

  memcpy(into, from, sizeof(struct Cell));
}

void eat_cell(struct Cell *from, struct Cell *into)
{

  int food = from->foodCode & from->life->acceptFood;

  if (food > 0 && from->foodQuantity > 0)
  {
    from->foodQuantity = 0;
    from->life->energy++;
  }
  memcpy(into, from, sizeof(struct Cell));
}

void simulation_step(struct Board from, struct Board to)
{

  int i, j, pos;

  for (i = 0; i < from.rows; i++)
  {
    for (j = 0; j < from.cols; j++)
    {

      pos = i * from.cols + j;

      if (from.board[pos].alive)
      {
        mutate_cell(&from.board[pos]);
        eat_cell(&from.board[pos], &to.board[pos]);
        kill_cell(&from.board[pos], &to.board[pos]);
      }
      else
        grow_cell(from, &to.board[pos], i, j);
    }
  }
}
