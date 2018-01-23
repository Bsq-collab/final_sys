#include "final.h"

struct host_info {
  int num_of_players;
  int has_password;
  char password[256];
};

void slow_print(char * str) {
  int last = 0;
  while ( str[last] ) {
    char part[256];
    memset(part, 0, 256);
    strncpy(part, str, last+1);
    usleep(100000);
    char *arg[1] = {"clear"};
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

struct host_info suggest_mkhost() {
  struct host_info mkhost_info;
  char y_or_n;

  printf("How many players?\n");
  scanf(" %d", &(mkhost_info.num_of_players));

  printf("Do you want to have a password? (y/n)\n");
  scanf(" %c", &y_or_n);

  if ( y_or_n == 'y' || 1 ) {
    mkhost_info.has_password = 1;
    printf("Password:\n");
    scanf("%s", mkhost_info.password);
  } else {
    mkhost_info.has_password = 0;
  }
  return mkhost_info;
}



int main() {
  //printf("@%c\n", ask("THIS IS A SAMPLE QUESTION"));
  suggest_mkhost();

  return 0;
}
