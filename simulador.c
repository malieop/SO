#include "unix.h"

#define MAXLINE 512

/* Servidor do tipo socket stream.
   Manda linhas recebidas de volta para o cliente */
//int newsockfd;












void * threadRoutine(newsockfd)
{
        char lineToWrite[50];
        sprintf(lineToWrite, "Olá, sou o cliente %i", pthread_self());
        printf("%s\n",lineToWrite);
}
void * virtualtime()
{
        while (simulador.aberto) {
                //gettime(auxtime);
                usleep(100000);
                simulador.contador_time++;

        }
}
void gettime( int auxtime) {

        int horas = 8;
        int min;

        horas = horas + auxtime/60;
        min = auxtime % 60;

        printf ("São %dh%dmin",horas, min);
}
void timersimulador()
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
        printf(", O Cliente nº%d chegou a bilheteria.\n",id);


}
void cria_cliente()
{
        for (int i = 0; i < simulador.max_cliente; i++) {
                usleep(150000);
                usleep(150000);
                usleep(150000);
                if((pthread_create(&(t_cliente[i]),NULL,(void *)trata_cliente, i))!=0)
                {
                        err_dump("pthread_create: erro criação thread");
                }
                else{
                        ;
                }
        }
}


void montanha_russa(int socket)
{
        int newsockfd = socket;

        pthread_t thread;
        if((pthread_create(&(thread),NULL,threadRoutine, (void *) newsockfd))!=0)
        {
                err_dump("pthread_create: erro criação thread");
        }
        else
        {
                char lineCreation[50];
                sprintf(lineCreation, "%i;%i;%c", pthread_self(), 3, 'e');
                printf("%s\n",  pthread_self());
                write((int )newsockfd, lineCreation, strlen(lineCreation));
        }
        pthread_join (thread, NULL);
}
