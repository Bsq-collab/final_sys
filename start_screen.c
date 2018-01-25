#include "final.h"
#include <ctype.h>

int get_int(int client_socket);

// int get_num_players(){
//   int n;
//   printf("Hello! Welcome to our beautiful final project\n");
//   printf("How many players? (2 to 4 players)\n");
//   n = get_int();
//   while(n > 4 || n < 2){
//     printf("Invalid input. Please try again.\n");
//     n = get_int();
//   }
//   return n;
// }

int get_int(int client_socket){
  char buffer[256];
  int n;
  process(client_socket, buffer, sizeof(buffer));
  sscanf(buffer,"%d",&n);
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

void parse_new_line(char ** buffer, char* line, char * parse_on){
  int i=0;
  while(i<10){
    char *f = strsep(&line,parse_on);
    // sscanf(strsep(&line,parse_on),"%s",buffer[i]);
    buffer[i] = f;
    // printf("%s\n", line);
    i+=1;
  }
}

void sprint_lines(char * buffer, char** lines, int number_of_lines, int line_num){
  // int i;
  // for (i=0 ; i<20 ; i++){
  //   printf("&&&\n");
  //   printf("[%d] %s\n", i, lines[i]);
  // }
  int i=line_num;
  char line[256];
  *buffer = 0;
  printf("@%s\n",buffer);
  while(i<number_of_lines+line_num){
    sprintf(line,"%s\n",lines[i]);
    printf("@@%s\n",line);
    strcat(buffer,line);
    i+=1;
  }
  strcat(buffer,"\n");
  printf("@@@%s\n",buffer);
}
int get_line(int client_socket, char** lines,int number_of_lines, int line_num){
  char output[256];
  memset(output, 0, 256);
  int i=line_num;
  int correct=0;
  while(i<number_of_lines+line_num){
    correct=atoi(lines[i]);
    sprintf(output,"HERES THE RIGHT ANS:%d\n",atoi(lines[i]));
    write(client_socket, output, sizeof(output));
    i+=1;
  }
  sprintf(output,"\n");
  write(client_socket, output, sizeof(output));
  return correct;
}
int get_user_num(int client_socket){
  char output[256];
  memset(output, 0, 256);
  int n;
  sprintf(output,"your answer(1-4):\n");
  write(client_socket, output, sizeof(output));
  n = get_int(client_socket);
  while(n > 4 || n < 1){
    sprintf(output,"Invalid input. Please try again.\n");
    write(client_socket, output, sizeof(output));
    n = get_int(client_socket);
  }
  return n;
}

int process(int client_socket, char * buffer, size_t buffersize) {
  int r = read(client_socket, buffer, buffersize);
  printf("[socket %d] received: [%s]\n", client_socket, buffer);
  return r;
}
void broadcast(int * client_socket, int num_of_players, char * buffer, size_t buffersize) {
  int i;
  for ( i=0 ; i<num_of_players ; i++ ) {
    write(client_socket[i], buffer, buffersize);
  }
}


//
// int old_main(){
//   //gets number of players
//   int num_players=0;
//   num_players=get_num_players();
//   while(num_players<2 ||  num_players>4){
//     printf("How many players?(2-4)\n");
//     scanf("%d",&num_players);
//     printf("number of players: %d\n",num_players);
//   }
//   int points[num_players];
//   memset(points,0,num_players);
//   int clearer=0;
//   while(clearer<4){
//     points[clearer]=0;
//     clearer+=1;
//   }
//   printf("points 0: %d\t 1: %d\t 2: %d\t\n",points[0],points[1],points[2]);
//   //asks for player names
//   //int i=1;
//   //char s[256][256];
//   //gets players names
//   /* while (i<=num_players){
//     printf("\n\n what's player %d's name? \n",i);
//     scanf("%s",s[i-1]);
//     printf("player %d: %s\n",i, s[i-1]);
//     i++;
//   }
//
//   //add a directions section if time. so that this is useful...
//   char start[256];
//   memset(start,0,256);
//
//   while(strcmp(start,"start")){
//     printf("type start to start:\n");
//     scanf("%s",start);
//   }*/
//   //for while loop
//   int current_question=0;
//   int current_answer=0;
//   //for the questions
//   char contents[1000];
//   char ** questions;
//   //for the answers
//   char choices[1000];
//   char ** answers;
//   //for the key
//   char key[1000];
//   char ** parsed_key;
//
//   //reading:
//   readfile("question.txt",contents,1000);
//   readfile("A.txt",choices,1000);
//   readfile("correct.txt",key,1000);
//   //parsing:
//   questions = parse_new_line(contents,"\n");
//   answers = parse_new_line(choices,"\n");
//   parsed_key = parse_new_line(key,"\n");
//
//   while(current_question<10){
//
//     print_lines(questions,1,current_question);
//     print_lines(answers,4,current_answer);
//
//     int LINE;
//     LINE = get_line(parsed_key,1,current_question);
//     // printf("\n\n\n\nLINE:_%d_\n\n",LINE);
//
//     int user_input = 0;
//     user_input = get_user_num();
//     printf("YOU PUT THIS AS YOUR ANSWER>: %d\n", user_input);
//     printf("user_input:_%d_\n\n",user_input);
//
//     if(user_input==LINE){
//       printf("\n\n\nCORRECT!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n");
//       points[0]+=100;
//       clearer=0;
//       while(clearer<num_players){
//         printf("player %d: %d\t", clearer+1, points[clearer]);
//         clearer+=1;
//       }
//       printf("\n");
//     }
//     else{
//       printf("\n\nINCORRECT\n\n The correct answer is: %d\n\n",LINE);
//     }
//
//     current_question++;
//     current_answer+=4;
//
//   }
//
//}
