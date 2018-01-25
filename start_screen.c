#include "final.h"
#include <ctype.h>

int get_int(int client_socket);

int get_int(int client_socket){
  char buffer[256];
  int n;
  process(client_socket, buffer, sizeof(buffer));
  sscanf(buffer,"%d",&n);
  return n;
}

void readfile(char* fileName, char* contents,int size ){

  int fd =open(fileName, O_RDONLY);
  read(fd, contents, size);
  close(fd);
}

void parse_new_line(char ** buffer, char* line, char * parse_on){
  int i=0;
  char * temp;
  while(temp = strsep(&line,parse_on)){
    strcpy(buffer[i], temp);
    i+=1;
  }
}

void sprint_lines(char * buffer, char** lines, int line_num){
  strcpy(buffer,lines[line_num]);
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
void broadcast(int * client_socket, int num_of_players, char * buffer, size_t buffersize,int*points, char ** names) {
  int i;
  char s[256];
  char temp[256];
  memset(s, 0, 256);
  memset(temp, 0, 256);
  strcat(s, "\n");
  for ( i=0 ; i<num_of_players ; i++ ) {
    sprintf(temp,"Player %s has %d points\n", names[i], points[i]);
    strcat(s,temp);
  }
  for ( i=0 ; i<num_of_players ; i++ ) {
    write(client_socket[i], buffer, buffersize);

    write(client_socket[i],s,sizeof(s));
  }

}
