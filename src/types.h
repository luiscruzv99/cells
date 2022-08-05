struct Bacteria {
  unsigned int mutation : 4;
  unsigned int reproduction : 4;
  unsigned int death : 4;
  unsigned int shape : 3;
  unsigned int color : 3;
  unsigned int maxEnergy : 4;
  unsigned int energy : 4;
  unsigned int acceptFood : 3;
  unsigned int name1 : 2;
  unsigned int name2 : 1;
};

struct Cell {
  unsigned int alive : 1;
  unsigned int foodCode : 3;
  unsigned int foodQuantity : 4;
  struct Bacteria *life;
};

struct Board{
    struct Cell * board;
    unsigned int rows;
    unsigned int cols;
    unsigned int uniqueFamilies;
};

const char* shapes = "xo-#@&<>";

const char* colors[] = {"\x1b[31;1m", "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m", "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m", "\x1b[0m"};
const char* bg_colors[] = {"\x1b[41m", "\x1b[45m", "\x1b[43m", "\x1b[42m", "\x1b[46m", "\x1b[44m", "\x1b[40m", "\x1b[0m"};


int BitsSetTable256[256];
