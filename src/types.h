struct Cell{
    unsigned int alive:1;
    unsigned int life:15;
};

struct Board{
    struct Cell * board;
    unsigned int rows;
    unsigned int cols;
};

const char* shapes = "xo-#@&<>";

const unsigned int grow_mask = 0b0110000000000000;
const unsigned int die_mask = 0b0001100000000000;
const unsigned int mutate_mask = 0b0000011100000000;
const unsigned int bg_mask = 0b0000000010000000;
const unsigned int color_mask = 0b0000000001111000;
const unsigned int shape_mask = 0b0000000000000111;
