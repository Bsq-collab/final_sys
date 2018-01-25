#include "networking.h"
#include "../final.h"
#include "../start_screen.c"


int main() {

  int listen_socket;
  // int client_socket;
  int f;
  int client_count = 0;
  char buffer[BUFFER_SIZE];
  int num_of_players = 4;

  char * names[num_of_players];
  memset(names, 0, num_of_players);
  int ready = 0;

  //set of file descriptors to read from
  fd_set read_fds;
  fd_set master;

  FD_ZERO(&read_fds);
  FD_ZERO(&master);

  char ** questions;
  char ** answers;
  char ** parsed_key;

  int current_question=0;
  int current_answer=0;

  int client_socket[num_of_players];
  memset(client_socket, 0, num_of_players);
  // int tochild_fds[num_of_players][2];
  // int toparent_fds[num_of_players][2];

  listen_socket = server_setup();
  int fdmax = listen_socket;

  FD_SET(STDIN_FILENO, &master); //add stdin to fd set
  FD_SET(listen_socket, &master); //add socket to fd set

  while (1) {
    read_fds = master;

    //select() modifies read_fds
    //we must reset it at each iteration
    // FD_ZERO(&read_fds); //0 out fd set

    //select will block until fd is ready
    select(fdmax + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
      client_socket[client_count] = server_connect(listen_socket);
      char output[BUFFER_SIZE];
      sprintf(output,"\n\n what's your name? \n");
      write(client_socket[client_count],output,sizeof(output));
      // pipe(tochild_fds[client_count]);
      // pipe(toparent_fds[client_count]);

      FD_SET(client_socket[client_count], &master);
      client_count++;

      // f = fork();
      // if (f == 0) {
      //   close(tochild_fds[client_count][WRITE]);
      //   close(toparent_fds[client_count][READ]);
      //   subserver(client_socket, toparent_fds[client_count]);
      // }
      // else {
      //   //add read end of pipe to fd set and update fdmax
      //   FD_SET(toparent_fds[client_count][READ], &master);
      //   if (toparent_fds[client_count][READ] > fdmax) fdmax = toparent_fds;
      //
      //   close(tochild_fds[client_count][READ]);
      //   close(toparent_fds[client_count][WRITE]);
      //   client_count++;
      //   close(client_socket);
      // }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      printf("[server] client count: %d\n", client_count);
    }//end stdin select

    //if a client triggered select
    char output[BUFFER_SIZE];
    int f;
    for ( f=0 ; f<client_count ; f++ ) {
      if (FD_ISSET(client_socket[f], &read_fds)) {
        printf("@@@\n");
        if (!names[f]) {
          process(client_socket[f], buffer, sizeof(buffer));
          sscanf(buffer,"%s",names[f]);
          sprintf(output,"player: %s\n", names[f]);
          write(client_socket[f], output, sizeof(output));
        }
        else if (ready) {
          get_q_and_a(questions,answers,parsed_key);

          print_lines(client_socket[f],questions,1,current_question);
          print_lines(client_socket[f],answers,4,current_answer);

          int LINE;
          LINE = get_line(client_socket[f],parsed_key,1,current_question);

          int user_input = 0;
          user_input = get_user_num(client_socket[f]);

          sprintf(output,"YOU PUT THIS AS YOUR ANSWER>: %d\n", user_input);
          write(client_socket[f], output, sizeof(output));

          sprintf(output,"user_input:_%d_\n\n",user_input);
          write(client_socket[f], output, sizeof(output));
          process(client_socket[f], buffer, sizeof(buffer));
          sscanf(buffer,"%d",&user_input);
          if(user_input==LINE){
            sprintf(output,"\n\n\nCORRECT!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n");
          }
          else{
            sprintf(output,"\n\nINCORRECT\n\n The correct answer is: %d\n\n",LINE);
          }
          write(client_socket[f], output, sizeof(output));

          current_question++;
          current_answer+=4;
        }
      }
    }

    ready = 1;
    for ( f=0 ; f<client_count ; f++ ) {
      if (!names[f]) ready = 0;
    }

  }
}

// //facillitate communication between the superserver and the client
// void subserver(int client_socket, int * toparent_fds) {
//   fd_set read_fds;
//   fd_set master;
//   FD_ZERO(&read_fds);
//   FD_ZERO(&master);
//   int fdmax = client_socket;
//   if (toparent_fds[READ] > fdmax) fdmax = toparent_fds[READ];
//   FD_SET(client_socket, &read_fds);
//   FD_SET(toparent_fds[READ], &read_fds)
//
//   char toclient[BUFFER_SIZE];
//   char toparent[BUFFER_SIZE];
//
//   while (1) {
//     read_fds = master;
//     select(fdmax + 1, &read_fds, NULL, NULL, NULL);
//     if (FD_ISSET(client_socket, &read_fds)) {
//       write(toparent_fds[WRITE], toparent, sizeof(toparent));
//     }
//     if (FD_ISSET(toparent_fds[READ], &read_fds)) {
//       write(client_socket, toclient, sizeof(toclient));
//     }
//   }
// }


void get_q_and_a(char ** questions, char ** answers, char ** parsed_key) {
  // char buffer[BUFFER_SIZE];
  // char output[BUFFER_SIZE];
  // int child;

  // //for testing client select statement
  // strncpy(buffer, "hello client", sizeof(buffer));
  // write(tochild_fds[WRITE], buffer, sizeof(buffer));

  // //gets number of players
  // int num_players=0;
  // while(num_players<2 ||  num_players>4){
  //   sprintf(output, "How many players? (2,3,4)\n");
  //   write(tochild_fds[WRITE],output,sizeof(output));
  //   process(tochild_fds[READ], buffer, sizeof(buffer));
  //   sscanf(buffer,"%d",&num_players);
  //   sprintf(output,"number of players: %d\n",num_players);
  //   write(tochild_fds[WRITE],output,sizeof(output));
  // }

  //}
  /*
  leave it commented out!!
  ------Unnecessary at the moment-------
  //add a directions section if time. so that this is useful...
  char start[256];
  memset(start,0,256);

  while(strcmp(start,"start")){
  sprintf(output,"type start to start:\n");
  write(tochild_fds[WRITE],output,sizeof(output));
  process(tochild_fds[READ], buffer, sizeof(buffer));
  sscanf(buffer,"%s",start);
  //}
  */
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
  questions = parse_new_line(contents,"\n");
  answers = parse_new_line(choices,"\n");
  parsed_key = parse_new_line(key,"\n");

}

// void ask_q_and_a (char ** questions, char ** answers, char ** parsed_key) {
//   //for while loop
//   int current_question=0;
//   int current_answer=0;
//   while(current_question<4){
//
//     print_lines(tochild_fds[WRITE],questions,1,current_question);
//     print_lines(tochild_fds[WRITE],answers,4,current_answer);
//
//     int LINE;
//     LINE = get_line(tochild_fds[WRITE],parsed_key,1,current_question);
//
//
//     int user_input = 0;
//     user_input = get_user_num(tochild_fds[WRITE]);
//
//     sprintf(output,"YOU PUT THIS AS YOUR ANSWER>: %d\n", user_input);
//     write(tochild_fds[WRITE], output, sizeof(output));
//
//     sprintf(output,"user_input:_%d_\n\n",user_input);
//     write(tochild_fds[WRITE], output, sizeof(output));
//     process(tochild_fds[READ], buffer, sizeof(buffer));
//     sscanf(buffer,"%d",&user_input);
//     if(user_input==LINE){
//       sprintf(output,"\n\n\nCORRECT!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n");
//     }
//     else{
//       sprintf(output,"\n\nINCORRECT\n\n The correct answer is: %d\n\n",LINE);
//     }
//     write(tochild_fds[WRITE], output, sizeof(output));
//
//     current_question++;
//     current_answer+=4;
//
//   }
//
//   /*
//   while (read(tochild_fds[WRITE], buffer, sizeof(buffer))) {
//
//   printf("[subserver %d] received: [%s]\n", getpid(), buffer);
//   process(buffer);
//   write(tochild_fds[WRITE], buffer, sizeof(buffer));
// }//end read loop
// */
// close(tochild_fds[WRITE]);
// exit(0);
// }
