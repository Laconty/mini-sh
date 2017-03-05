#include <stdio.h>
#include <stdlib.h>

#include "mini-sh.h"

int main (int argc, char **argv) {
  msh_loop();

  printf("Main after lol\n");
  return EXIT_SUCCESS;
}
