#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <semaphore.h>

#define UNIXSTR_PATH "/tmp/s.unixstr"
#define UNIXDG_PATH  "/tmp/s.unixdgx"
#define UNIXDG_TMP   "/tmp/dgXXXXXXX"

struct estatisticas
{
        int total_parque;
        int total_montanha1;
        int total_montanha2;
        int total_montanha3;
        int tempo_desistencias;
        int total_desistencias;
};

//---Prototypes--//
//---util.c
void err_dump(char *msg);
int readn(int fd, char *ptr, int nbytes);
int writen(int fd, char *ptr, int nbytes);
int readline(int fd, char *ptr, int maxlen);
void ecra(int sockfd);
void montanha_russa(int sockfd);
char * protocologoComunicacao(char * codigo, char * separador);
struct estatisticas atualizaDadosEstatisticas(char * codigo, char * separador, struct estatisticas stats);


//variaveis globais
int conta_cliente; 

//
struct parque
{

        int max_cliente;
        int bilhetaria;
        int contador_time;
        int aberto;
        int perc_prioridade;

} ;
struct cliente_struct
{
        int id;
        int prioridade;
        int tempo_chegou;
        int tempo_saiu;
        int tempo_esperado;
        int divertimento;


};
// semaforos
sem_t s_tam_fila_bilheteira;
sem_t s_tam_max_parque;

struct cliente_struct cliente[50000];
struct parque simulador;
pthread_t static t_cliente [50000];
