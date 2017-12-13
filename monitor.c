#include "unix.h"
#define MAXLINE 512
//ESTOU AQUI
void ecra(int sockfd)
{
	char line[MAXLINE];
	int n;
	char sendline[MAXLINE];

	if((n = read(sockfd, line, MAXLINE-1)) > 0)
	{
		printf("%s\n",line);

		char * mensagemCompleta = protocologoComunicacao(line, ";");
		printf("%s\n", mensagemCompleta);
		bzero(line,MAXLINE);
	}
	else if(n<0) printf("ERRO LENDO DA SOCKFD");
}
