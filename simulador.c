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
