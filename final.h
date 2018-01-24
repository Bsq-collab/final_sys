#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include  <sys/ipc.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <ctype.h>

#define PORT_NUM "6743"

//Start_screen fxn names:
int get_num_players();
int get_int(int client_socket);
void readfile(char* fileName, char* contents,int size );
char ** parse_new_line(char* line,char * parse_on);
void print_lines(int client_socket, char** lines, int number_of_lines, int line_num);
int get_line(int client_socket, char** lines,int number_of_lines, int line_num);
int get_user_num(int client_socket);
void process(int client_socket, char * buffer, size_t buffersize);

//Lim_Player fxn names:
void slow_print(char * str);
char ask(char * q_and_a);
