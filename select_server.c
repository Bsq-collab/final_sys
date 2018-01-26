#include "networking.h"
#include "final.h"
#include "start_screen.c"

void die(int * client_socket) {
  int i;
  for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
    char buf[256] = "cyalater";
    write(client_socket[i], buf, sizeof(buf));
    close(client_socket[i]);
    shutdown(client_socket[i],2);
  }
  exit(0);
}

int main() {
  int listen_socket;
  int i;
  int client_count = 0;
  char buffer[BUFFER_SIZE];
  char ans_buf[BUFFER_SIZE];

  // char names[NUM_PLAYERS][BUFFER_SIZE];
  // for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
  //   memset(names[i], 0, BUFFER_SIZE);
  // }
  char *names[NUM_PLAYERS];
  for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
    names[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
  }


  int ready = 0;

  //set of file descriptors to read from
  fd_set read_fds;
  fd_set master;

  FD_ZERO(&read_fds);
  FD_ZERO(&master);

  char *questions[BUFFER_SIZE];
  char *answers[BUFFER_SIZE];
  char *parsed_key[BUFFER_SIZE];
  for ( i=0 ; i<BUFFER_SIZE ; i++ ) {
    questions[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
    answers[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
    parsed_key[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
  }
  get_q_and_a(questions, answers, parsed_key);

  int current_question=0;
  int current_answer=0;

  int client_socket[NUM_PLAYERS];
  int points[NUM_PLAYERS];
  memset(client_socket, 0, NUM_PLAYERS);
  memset(points, 0, NUM_PLAYERS);

  listen_socket = server_setup();
  int fdmax = listen_socket;

  FD_SET(STDIN_FILENO, &master); //add stdin to fd set
  FD_SET(listen_socket, &master); //add socket to fd set

  while (1) {
    read_fds = master;

    //select will block until fd is ready
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(1);
    }

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
    }//end stdin select

    //if a client triggered select
    for ( i=0 ; i<client_count ; i++ ) {
      if (FD_ISSET(client_socket[i], &read_fds)) {
        if ( process(client_socket[i], ans_buf, sizeof(ans_buf)) ) {
          if (!ready) {
            sscanf(ans_buf, "%s\n", names[i]);
            sprintf(buffer, "hello %s\n", names[i]);
            write(client_socket[i], buffer, sizeof(buffer));
            sprintf(buffer, "game will start when all players join\n");
            write(client_socket[i], buffer, sizeof(buffer));

            ready = 1;
            for ( i=0 ; i<NUM_PLAYERS ; i++ ) {
              if ( !strcmp(names[i],"") ) ready = 0;
            }
            if(ready){
              strcpy(buffer, questions[current_question]);

              broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer),points, (char **)names);
              strcpy(buffer, answers[current_question]);

              broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer),points,(char **)names);
              strcpy(ans_buf, parsed_key[current_question]);
            }
          }
          else if (ready && current_question < 10)  {
            int answer_user=-1;
            sscanf(ans_buf,"%d\n",&answer_user);


            if(answer_user==atoi(parsed_key[current_question])){
              strcpy(buffer, "GOOD WOORK\n");
              broadcast(client_socket,NUM_PLAYERS, buffer,sizeof(buffer),points,(char **)names);
              points[i]+=109;
              current_question+=1;
              answer_user=-1;
            }
            else{
              printf("YOU WRONGO BRO");
            }
            strcpy(buffer, questions[current_question]);
            broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer),points,(char **)names);

            strcpy(buffer, answers[current_question]);
            broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer),points,(char **)names);
          }
          if (current_question == 10) {
            int max = 0;
            int max_player = 0;
            int j;
            for ( j=0 ; j<NUM_PLAYERS ; j++ ) {
              if (points[j] > max){
                max = points[j];
                max_player = j;
              }
            }
            sprintf(buffer, "AND THE WINNER IS: %s\n", names[max_player]);
            broadcast(client_socket, NUM_PLAYERS, buffer, sizeof(buffer),points,(char **)names);
            die(client_socket);
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
  readfile("question.txt",contents,1000);
  readfile("A.txt",choices,1000);
  readfile("correct.txt",key,1000);
  //parsing:
  parse_new_line(questions,contents,"\n");
  parse_new_line(answers,choices,"\n");
  parse_new_line(parsed_key,key,"\n");

}
