#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define UNIXSTR_PATH "/tmp/s.unixstr"
#define UNIXDG_PATH  "/tmp/s.unixdgx"
#define UNIXDG_TMP   "/tmp/dgXXXXXXX"

//constantes

#define num_bilheteiras 4

struct estatisticas
{
        int total_parque;
        int total_bilhetes_vendidos;
        int total_montanha1;
        int total_montanha2;
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
char * protocologoComunicacao(char * codigo, char * separador);
struct estatisticas atualizaDadosEstatisticas(char * codigo, char * separador, struct estatisticas stats);
void *cria_cliente(int socket);
void *cria_bilheteira(int socket);
void * takabisha (int socket);
void * fury325(int socket);
void *divertimento();
void * virtualtime();


//variaveis globais
int conta_cliente;
int conta_bilheteira;
int produz_bilheteira;
int consome_bilheteira;  // variaveis para o produtores consumidores da bilheteira
int num_prio_takabisha;
int num_sem_prio_takabisha;
int num_pessoas_fury325;
int verifica_cliente_takabisha;
//
struct parque
{

        int max_cliente;
        int bilhetaria;
        int contador_time;
        int aberto;
        int hora_de_abertura;
        int hora_de_fecho;
        int tempo_aberto; // simula os minutos que estará ano diggityberto
        int perc_prioridade;
        int perc_cliente_bilhete; // percentagem de clientes que compras bilhete
        int buff_bilheteira[num_bilheteiras];
        int divertimento_boolean; // meter função para fechar os divertimentos !!!

};
struct cliente_struct
{
        int id;
        int prioridade;
        int tempo_chegou_bilheteira;
        int tempo_chegou_divertimento;
        int tempo_saiu;
        int tempo_esperado;
        int tempo_max_espera;
        int divertimento;
        int bilheteira;


};
// semaforos
sem_t s_tam_fila_bilheteira;
sem_t s_tam_max_parque;
sem_t s_prod_bilheteira;
sem_t s_cons_bilheteira;
sem_t s_takabisha;
sem_t s_prio_takabisha;
sem_t s_sem_prio_takabisha;
sem_t s_finish_takabisha;
sem_t s_finish_fury;
sem_t s_cliente_fury325;
sem_t s_fury325;
sem_t s_comunicacaofury325;
sem_t s_comunicacao_takabisha;
sem_t s_sai_takabisha;
sem_t s_cliente_tempo;
sem_t s_cliente_verificado;



struct cliente_struct cliente[50000];
struct parque simulador;
pthread_t static t_cliente [50000];
pthread_t thread;
pthread_t t_bilheteira[1000];
pthread_t t_cria_cliente;
pthread_t t_cria_bilheteira;
pthread_t t_cria_takabisha;
pthread_t t_cria_fury;
pthread_t t_cria_booleano_divertimento;
pthread_mutex_t mutex_bilheteira;
pthread_mutex_t mutex_takabisha;
pthread_mutex_t mutex_fury;
pthread_mutex_t mutex_comunicacao;
