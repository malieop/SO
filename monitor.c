#include "unix.h"
#define MAXLINE 512

struct estatisticas
{
        int total_parque;
        int total_montanha1;
        int total_montanha2;
        int total_montanha3;
        int tempo_desistencias;
        int total_desistencias;
};
void * atualizaDadosEstatisticas(char * codigo, char * separador, struct estatisticas stats)
{
//         strtok(codigo, separador); //id do cliente, não é necessário
//
//         switch (strtol(strtok(NULL, separador),NULL,0)) { //strtol - converte char para inteiro
//         case 1:         //entrou
//                 switch (*strtok(NULL, separador)) {
//                 case 'a':
//                         break;
//                 case 'b': stats.total_parque++;
//                         break;.
//                 case 'c': stats.total_montanha1++;
//                         break;
//                 case 'd': stats.total_montanha2++;
//                         break;
//                 case 'e': stats.total_montanha3++;
//                         break;
//                 }
//                 break;
//         case 4:        //desistiu
//                 stats.tempo_desistencias += 10;
//                 stats.total_desistencias++;
//                 break;
//         }
//
                stats.total_parque = 20;
                printf ("x = %d, y = %d", stats.total_parque, stats.total_montanha1);
}

void ecra(int sockfd){

        struct estatisticas stats = {0, 0, 0, 0, 0, 0};

        char line[MAXLINE];
        int n;
        char sendline[MAXLINE];

        if((n = read(sockfd, line, MAXLINE-1)) > 0)
        {
                printf("%s\n",line);

                atualizaDadosEstatisticas(line, ";", stats);
                char * mensagemCompleta = protocologoComunicacao(line, ";");
                printf("%s\n", mensagemCompleta);
                bzero(line,MAXLINE);
        }
        else if(n<0) printf("ERRO LENDO DA SOCKFD");


}
