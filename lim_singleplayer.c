#include "final.h"

void slow_print(char * str) {
  int last = 0;
  while ( str[last] ) {
    char part[256];
    memset(part, 0, 256);
    strncpy(part, str, last+1);
    usleep(100000);
    char arg[1][5] = {"clear"};
    //execlp(arg, "clear");
    printf("%s\n", part);
    last++;
  }
}

char ask(char * q_and_a) {
  printf("%s\n", q_and_a);
  //slow_print(q_and_a);
  char buf[256];
  fgets(buf, 256, stdin);
  printf("Your answer: ");
  fgets(buf, 256, stdin);
  return *buf;
}

int main() {
  printf("@%c\n", ask("THIS IS A SAMPLE QUESTION"));
  
  return 1;
}
