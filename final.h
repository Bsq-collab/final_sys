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
#define READ 0
#define WRITE 0
#define NUM_PLAYERS 2

//Start_screen fxn names:
int get_num_players();
int get_int(int client_socket);
void readfile(char* fileName, char* contents,int size );
void parse_new_line(char ** buffer, char* line, char * parse_on);
void sprint_lines(char * buffer, char** lines, int line_num);
void print_lines(int client_socket, char** lines, int number_of_lines, int line_num);
int get_line(int client_socket, char** lines,int number_of_lines, int line_num);
int get_user_num(int client_socket);
int process(int client_socket, char * buffer, size_t buffersize);
void broadcast(int * client_socket, int num_of_players, char * buffer, size_t buffersize,int*points, char ** names);
void get_q_and_a(char ** questions, char ** answers, char ** parsed_key);

//Lim_Player fxn names:
void slow_print(char * str);
char ask(char * q_and_a);
