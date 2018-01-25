#include "networking.h"
#include "../final.h"
#include "../start_screen.c"

void die(int * client_socket) {
  int i;
  for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
    close(client_socket[i]);
  }
  exit(0);
}

int main() {
  int listen_socket;
  int i;
  int client_count = 0;
  char buffer[BUFFER_SIZE];

  char names[NUM_PLAYERS][BUFFER_SIZE];
  for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
    memset(names[i], 0, BUFFER_SIZE);
  }
  int ready = 0;

  //set of file descriptors to read from
  fd_set read_fds;
  fd_set master;

  FD_ZERO(&read_fds);
  FD_ZERO(&master);

  // char questions[BUFFER_SIZE][BUFFER_SIZE];
  // char answers[BUFFER_SIZE][BUFFER_SIZE];
  // char parsed_key[BUFFER_SIZE][BUFFER_SIZE];
  // for ( i=0 ; i<BUFFER_SIZE ; i++ ) {
  //   memset(questions[i], 0, BUFFER_SIZE);
  //   memset(answers[i], 0, BUFFER_SIZE);
  //   memset(parsed_key[i], 0, BUFFER_SIZE);
  // }
  // get_q_and_a((char **)questions, (char **)answers, (char **)parsed_key);
  char *questions[BUFFER_SIZE];
  char *answers[BUFFER_SIZE];
  char *parsed_key[BUFFER_SIZE];
  for ( i=0 ; i<BUFFER_SIZE ; i++ ) {
    // char tempq[BUFFER_SIZE];
    // memset(tempq, 0, BUFFER_SIZE);
    // questions[i] = tempq;
    // char tempa[BUFFER_SIZE];
    // memset(tempa, 0, BUFFER_SIZE);
    // answers[i] = tempa;
    // char tempk[BUFFER_SIZE];
    // memset(tempk, 0, BUFFER_SIZE);
    // parsed_key[i] = tempk;
    questions[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
    answers[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
    parsed_key[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
  }
  // char ** questions = (char **)calloc(BUFFER_SIZE, sizeof(char *));
  // char ** answers = (char **)calloc(BUFFER_SIZE, sizeof(char *));
  // char ** parsed_key = (char **)calloc(BUFFER_SIZE, sizeof(char *));
  get_q_and_a(questions, answers, parsed_key);
  printf("*********%s\n",answers[5]);

  int current_question=0;
  int current_answer=0;

  int client_socket[NUM_PLAYERS];
  memset(client_socket, 0, NUM_PLAYERS);

  listen_socket = server_setup();
  int fdmax = listen_socket;

  FD_SET(STDIN_FILENO, &master); //add stdin to fd set
  FD_SET(listen_socket, &master); //add socket to fd set

  while (1) {
    read_fds = master;

    //select will block until fd is ready
    select(fdmax + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
      client_socket[client_count] = server_connect(listen_socket);
      sprintf(buffer, "type your name\n");
      write(client_socket[client_count], buffer, sizeof(buffer));

      FD_SET(client_socket[client_count], &master);
      if (client_socket[client_count] > fdmax) {
        fdmax = client_socket[client_count];
      }
      client_count++;
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, BUFFER_SIZE, stdin);
      die(client_socket);
      // printf("[server] client count: %d\n", client_count);
    }//end stdin select

    //if a client triggered select
    for ( i=0 ; i<client_count ; i++ ) {
      if (FD_ISSET(client_socket[i], &read_fds)) {
        if ( process(client_socket[i], buffer, sizeof(buffer)) ) {
          if (!ready) {
            sscanf(buffer, "%s\n", names[i]);
            sprintf(buffer, "hello %s\n", names[i]);
            write(client_socket[i], buffer, sizeof(buffer));
            sprintf(buffer, "game will start when all players join\n");
            write(client_socket[i], buffer, sizeof(buffer));

            ready = 1;
            for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
              if ( !strcmp(names[i],"") ) ready = 0;
            }
          }
          if (ready && current_question < 10)  {
            printf("@@@@@@@@@\n");
            memset(buffer, 0, BUFFER_SIZE);
            strcpy(buffer, questions[current_question]);
            // sprint_lines(buffer,(char **)questions,current_question);
            broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer));
            strcpy(buffer, answers[current_question]);
            // sprint_lines(buffer,(char **)answers, current_question);
            broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer));
            current_question+=1;
            //current_answer+=4;
          }
        }
      }
    }


  }
}
void get_q_and_a(char ** questions, char ** answers, char ** parsed_key) {
  //for the questions
  char contents[1000];
  //for the answers
  char choices[1000];
  //for the key
  char key[1000];

  //reading:
  readfile("../question.txt",contents,1000);
  readfile("../A.txt",choices,1000);
  readfile("../correct.txt",key,1000);
  //parsing:
  parse_new_line(questions,contents,"\n");
  parse_new_line(answers,choices,"\n");
  parse_new_line(parsed_key,key,"\n");

}
