#include "networking.h"
#include "../final.h"

static void sighandler(int signo){
  if (signo == SIGPIPE){
    exit(0);
  }
}

int main(int argc, char **argv) {
  signal(SIGPIPE, sighandler);

  int server_socket;
  char buffer[BUFFER_SIZE];

  fd_set read_fds;

  if (argc == 2)
  server_socket = client_setup( argv[1]);
  else
  server_socket = client_setup( TEST_IP );

  while (1) {

    // printf("enter data: ");
    // //the above printf does not have \n
    // //flush the buffer to immediately print
    // fflush(stdout);

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    // select(server_socket + 1, &read_fds, NULL, NULL, NULL);
    if (select(server_socket+1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(1);
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      write(server_socket, buffer, sizeof(buffer));
      read(server_socket, buffer, sizeof(buffer));
      printf("%s\n", buffer);
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      read(server_socket, buffer, sizeof(buffer));
      if (!strcmp(buffer,"cyalater")) {
        exit(0);
      }
      printf("[SERVER BROADCAST] %s\n", buffer);
      printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);
    }//end socket select

  }//end loop
}
