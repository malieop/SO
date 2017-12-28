#include "unix.h"

#define MAXLINE 512

int newsockfd;

void * threadRoutine(newsockfd)
{
        char lineToWrite[50];
        sprintf(lineToWrite, "Olá, sou o cliente %i", pthread_self());
        printf("%s\n",lineToWrite);
}
void * virtualtime() // função que incrementa variavel do tempo virtual
{
        while (simulador.aberto) {
                //gettime(auxtime);
                usleep(1500000);
                simulador.contador_time++;

        }
}
void gettime( int auxtime) { // funçao para obter o tempo virtual

        int horas = 8;
        int min;

        horas = horas + auxtime/60;
        min = auxtime % 60;

        printf ("São %dh%dmin",horas, min);
}
void timersimulador() // cria a thread para a contagem do tempo virtual
{
        pthread_t thread;
        if((pthread_create(&(thread),NULL,(void *)virtualtime, NULL))!=0)
        {
                err_dump("pthread_create: erro criação thread");
        }
        else printf("criou timer\n" );
}
void *trata_cliente(int id)
{ //para usar o rand()

        time_t t;
        srand((unsigned) time(&t));
        gettime( simulador.contador_time);
        char lineBilheteria [50];
        sprintf(lineBilheteria, "%d;2;a", id);
        printf(", O Cliente nº%d chegou a bilheteria.\n",id);

        write(newsockfd, lineBilheteria, strlen(lineBilheteria));

        if((rand()%100)< simulador.perc_prioridade) // define se o cliente é prioritário ou não
        {cliente[id].prioridade = 1; }
        else
        {cliente[id].prioridade = 0; }
        //METER SEMAFORO DO TAMANHO tam_fila_bilheteiraDA FILA DE ESPERA PARA ENTRAR NO PARQUE

        sem_wait(&s_tam_fila_bilheteira);

        cliente[id].divertimento = rand()%3;

        if(cliente[id].prioridade == 1)
        {
                gettime(simulador.contador_time);
                printf(", O Cliente nº%d, sem prioridade, entrou no parque.\n",id);
        }
        else
        {
                gettime(simulador.contador_time);
                printf(", O Cliente nº%d,com prioridade, entrou no parque.\n",id);
        }
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        usleep(150000);
        sem_post(&s_tam_fila_bilheteira);


}
void cria_cliente(int socket)
{
        newsockfd = socket;
        for (int i = 0; i < simulador.max_cliente; i++) {
                usleep(1500000);
                conta_cliente++;

                if((pthread_create(&(t_cliente[i]),NULL,(void *)trata_cliente, i))!=0)
                {
                        err_dump("pthread_create: erro criação thread");
                }
                else{
                        char lineCriacao [50];
                        sprintf(lineCriacao, "%d;1;a", i);
                        printf("O Cliente nº%d chegou ao parque.\n",i);
                        write(newsockfd, lineCriacao, strlen(lineCriacao));
                }
        }
}


void montanha_russa(int socket)
{
        // newsockfd = socket;
        //
        // pthread_t thread;
        // else
        // {
        //         char lineCreation[50];
        //         sprintf(lineCreation, "%i;%i;%c", pthread_self(), 3, 'e');
        //         printf("%s\n",  pthread_self());
        //         write((int )newsockfd, lineCreation, strlen(lineCreation));
        // }
        // pthread_join (thread, NULL);
}
