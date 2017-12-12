#include "unix.h"
#define MAXLINE 512

void ecra(int sockfd)
{
        struct estatisticas stats = {0, 0, 0, 0, 0, 0};

        char line[MAXLINE];
        int n;
        char sendline[MAXLINE];

        if((n = read(sockfd, line, MAXLINE-1)) > 0)
        {
                //é preciso porque partimos a string duas vezes, e não podemos fazer no mesmo apontador
                char * auxLine = (char *) malloc(sizeof(char) * 3); //aloca memoria para auxLine
                strcpy(auxLine, line); // copia a line para a auxLine

                stats = atualizaDadosEstatisticas(line, ";", stats); //atualiza a estrutura consoante a mensagem recebida

                char * mensagemCompleta = protocologoComunicacao(auxLine, ";"); //'descodifica' a mensagem recebida
                printf("%s\n", mensagemCompleta);
                bzero(line,MAXLINE);
        }
        else if(n<0) printf("ERRO LENDO DA SOCKFD");


}
