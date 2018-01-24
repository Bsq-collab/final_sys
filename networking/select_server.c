#include "networking.h"
#include "../final.h"
#include "../start_screen.c"

void process(int client_socket, char * buffer, size_t buffersize);
void subserver(int from_client);

int main() {

  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     client_socket = server_connect(listen_socket);

     f = fork();
     if (f == 0)
       subserver(client_socket);
     else {
       subserver_count++;
       close(client_socket);
     }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      printf("[server] subserver count: %d\n", subserver_count);
    }//end stdin select
  }
}


void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];
  char output[BUFFER_SIZE];

  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));
  //gets number of players
  int num_players=0;
  while(num_players<2 ||  num_players>4){
    sprintf(output, "How many players? (2,3,4)\n");
    write(client_socket,output,sizeof(output));
    process(client_socket, buffer, sizeof(buffer));
    sscanf(buffer,"%d",&num_players);
    sprintf(output,"number of players: %d\n",num_players);
    write(client_socket,output,sizeof(output));
  }

  //asks for player names
  int i=1;
  char s[256][256];
  //gets players names
  while (i<=num_players){
    sprintf(output,"\n\n what's player %d's name? \n",i);
    write(client_socket,output,sizeof(output));
    process(client_socket, buffer, sizeof(buffer));
    sscanf(buffer,"%s",s[i-1]);
    sprintf(output,"player %d: %s\n",i, s[i-1]);
    write(client_socket, output, sizeof(output));
    i++;
  }
  /*
    leave it commented out!!
    ------Unnecessary at the moment-------
  //add a directions section if time. so that this is useful...
  char start[256];
  memset(start,0,256);

  while(strcmp(start,"start")){
    sprintf(output,"type start to start:\n");
    write(client_socket,output,sizeof(output));
    process(client_socket, buffer, sizeof(buffer));
    sscanf(buffer,"%s",start);
  }
  */
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

  //reading:
  readfile("../question.txt",contents,1000);
  readfile("../A.txt",choices,1000);
  readfile("../correct.txt",key,1000);
  //parsing:
  questions = parse_new_line(contents,"\n");
  answers = parse_new_line(choices,"\n");
  parsed_key = parse_new_line(key,"\n");

  while(current_question<4){

    print_lines(client_socket,questions,1,current_question);
    print_lines(client_socket,answers,4,current_answer);

    int LINE;
    LINE = get_line(client_socket,parsed_key,1,current_question);\
    // printf("\n\n\n\nLINE:_%d_\n\n",LINE);

    int user_input = 0;
    user_input = get_user_num(client_socket);
    sprintf(output,"YOU PUT THIS AS YOUR ANSWER>: %d\n", user_input);
    write(client_socket, output, sizeof(output));
    sprintf(output,"user_input:_%d_\n\n",user_input);
    write(client_socket, output, sizeof(output));

    if(user_input==LINE){
      sprintf(output,"\n\n\nCORRECT!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n");
    }
    else{
      sprintf(output,"\n\nINCORRECT\n\n The correct answer is: %d\n\n",LINE);
    }
    write(client_socket, output, sizeof(output));

    current_question++;
    current_answer+=4;

  }

  /*
  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  */
  close(client_socket);
  exit(0);
}

void process(int client_socket, char * buffer, size_t buffersize) {
  read(client_socket, buffer, buffersize);
  printf("[subserver %d] received: [%s]\n", getpid(), buffer);
}
