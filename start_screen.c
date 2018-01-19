#include "final.h"
#include <ctype.h>

int get_int();

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
void readfile(char* fileName, char* contents,int size ){

  int fd =open(fileName, O_RDONLY);
  read(fd, contents, size);
}

char ** parse_new_line(char* line,char * parse_on){
  char ** retans = malloc(256* sizeof(char*));
  int i=0;
  while(line){
    retans[i]=strsep(&line,parse_on);
    i+=1;
  }
  return retans;
}

void print_lines(char** lines, int number_of_lines, int line_num){
  int i=line_num;
  while(i<number_of_lines+line_num){
    printf("%s\n",lines[i]);
    i+=1;
  }
  printf("\n");
}
void get_user_input(char user_answer[4]){
  //char user_answer[2];
  printf("Your answer:\n");
  fgets(user_answer, 4, stdin);
  printf("user_answer: %s\n",user_answer);
  //return user_answer;
}
int main(){
  /*  //gets number of players
  int num_players=0;
  num_players=get_num_players();
  while(num_players<2 ||  num_players>4){
    printf("How many players?(2-4)\n");
    scanf("%d",&num_players);
    printf("number of players: %d\n",num_players);
    }*/
  /*
  //asks for player names
  int i=1;
  char s[256][256];
  //gets players names
  while (i<=num_players){
    printf("\n\n what's player %d's name? \n",i);
    scanf("%s",s[i-1]);
    printf("player %d: %s\n",i, s[i-1]);
    i++;
  }
  */
  /*//add a directions section if time. so that this is useful...
char start[256];
  memset(start,0,256);

  while(strcmp(start,"start")){
    printf("type start to start:\n");
    scanf("%s",start);
    }*/
  //for while loop
  int current_question=0;
  int current_answer=0;
  //for the questions
  char contents[1000];
  char ** questions;
  //for the answers
  char choices[1000];
  char ** answers;
  //for the key
  char key[1000];
  char ** parsed_key;

  char user_answer[4];
  //reading:
  readfile("question.txt",contents,1000);
  readfile("A.txt",choices,1000);
  readfile("correct.txt",key,1000);
  //parsing:
  questions = parse_new_line(contents,"\n");
  answers = parse_new_line(choices,"\n");
  parsed_key = parse_new_line(key,"\n");

  while(current_question<4){
    print_lines(questions,1,current_question);
    print_lines(answers,4,current_answer);
    get_user_input(user_answer);
    printf("useranswer: %s\n",user_answer );
    current_question++;
    current_answer+=4;

  }



}
