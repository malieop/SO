#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define UNIXSTR_PATH "/tmp/s.unixstr"
#define UNIXDG_PATH  "/tmp/s.unixdgx"
#define UNIXDG_TMP   "/tmp/dgXXXXXXX"

//---Prototypes--//
//---util.c
void err_dump(char *msg);
int readn(int fd, char *ptr, int nbytes);
int writen(int fd, char *ptr, int nbytes);
int readline(int fd, char *ptr, int maxlen);
void ecra(int sockfd);
void montanha_russa(int sockfd);

//variaveis globais
struct parque
{

        int max_cliente;
        int contador_time;
        int aberto;

} ;
struct cliente_struct
{
        int id;
        int vip;
        int prioridade;
        int tempo_chegou;
        int tempo_saiu;
        int tempo_esperado;


};


struct cliente_struct cliente[50];
struct parque simulador;
pthread_t static t_cliente [50];
