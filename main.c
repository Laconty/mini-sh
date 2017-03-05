#include <stdio.h>
#include <stdlib.h>

#include "mini-sh.h"

int main (int argc, char **argv) {
  lsh_loop();

  printf("Main after lol\n");
  return EXIT_SUCCESS;
}
