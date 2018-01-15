#include "final.h"
#include <ctype.h>

/*int get_int();

int get_num_players(){
  int n;
  printf("Hello! Welcome to our beautiful final project\n");
  printf("How many players? (2 to 4 players)\n");
  n = get_int();
  while(n > 4 || n < 2){
    printf("Invalid input. Please try again.\n");
    n = get_int();
  }
  return n;
}

int get_int(){
  char buff[256];
  int n;
  fgets(buff, 256, stdin);
  n = strtol(buff, NULL, 10);
  return n;
}

/*int main(){
  int n;
  n = get_num_players();
  printf("%d\n", n);
  return 0;
  }*/

int main(){
  int num_players=0;

  while((!isdigit(num_players))&&(num_players<2 ||num_players>4)){
    printf("How many players?(2-4)\n");
    scanf("%d",&num_players);
    printf("number of players: %d\n",num_players);
  }

}
