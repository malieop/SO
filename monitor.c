#include "unix.h"
#define MAXLINE 512
#define SIZEARRAY 17
//ESTOU AQUI
int acertaStatsN(int stat)
{
        if (stat == 0) return 1;
        else return floor (log10 (abs (stat))) + 1;
}
void printCab()
{
        printf("   ┌─────────────────────────────────────────────────────────────────────────┐\n");
        printf("   │                          DISNEYLAND MADEIRA                             │\n");
        printf("   │                    *Sistemas Operativos 2017/2018*                      │\n");
        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");
}

void printStats(struct estatisticas stats)
{
        int aux;
        printf("   ├─[Estatísticas]──────────────────────────────────────────────────────────┤\n");

        if(acertaStatsN(stats.total_bilhetes_vendidos) <= 20) //confirma que não passa do limite superado pela pagina
        {
                //calcula o espaço vazio em seguida ao números
                if(21-acertaStatsN(stats.total_parque) < 0) aux = 0;
                else aux = 21-acertaStatsN(stats.total_parque);
                printf("   │• Número total de pesssoas no parque hoje: %d pessoas %*s│\n",stats.total_parque, aux, " ");
        }
        else //caso número seja muito grande, faz print do maior número superado
        {
                printf("   │• Número total de pesssoas no parque hoje: +99999999999999999999 pessoas │\n");
        }

        if(acertaStatsN(stats.total_bilhetes_vendidos) <= 31)
        {
                if(32-acertaStatsN(stats.total_bilhetes_vendidos) < 0) aux = 0;
                else aux = 32-acertaStatsN(stats.total_bilhetes_vendidos);
                printf("   │• Número de bilhetes vendidos: %d bilhetes %*s│\n", stats.total_bilhetes_vendidos, aux, " ");
        }
        else
        {
                printf("   │• Número de bilhetes vendidos: +9999999999999999999999999999999 pessoas │\n");
        }

        if(acertaStatsN(stats.total_montanha1) <= 4)
        {
                if(5-acertaStatsN(stats.total_montanha1) < 0) aux = 0;
                else aux = 5-acertaStatsN(stats.total_montanha1);
                printf("   │• Número de pessoas que andou na montanha russa Takabisha: %d pessoas %*s│\n",stats.total_montanha1, aux, " ");
        }
        else
        {
                printf("   │• Número de pessoas que andou na montanha russa Fury 325: +9999 pessoas │\n");
        }

        if(acertaStatsN(stats.total_montanha2) <= 5)
        {
                if(6-acertaStatsN(stats.total_montanha2) < 0) aux = 0;
                else aux = 6-acertaStatsN(stats.total_montanha2);
                printf("   │• Número de pessoas que andou na montanha russa Fury 325: %d pessoas %*s│\n",stats.total_montanha2, aux, " ");
        }
        else
        {
                printf("   │• Número de pessoas que andou na montanha russa Fury 325: +99999 pessoas │\n");

        }

        if(acertaStatsN(stats.total_montanha2) <= 32)
        {
                if(33-acertaStatsN(stats.total_montanha2) < 0) aux = 0;
                else aux = 33-acertaStatsN(stats.total_montanha2);
                printf("   │• Número total de desistentes: %d pessoas %*s│\n",stats.total_montanha2, aux, " ");
        }
        else
        {
                printf("   │• Número total de desistentes: +99999999999999999999999999999999 pessoas │\n");
        }

        // printf("   │• Número de pessoas que andou na montanha russa Takabisha: %d pessoas %*s│\n", stats.total_desistencias);
        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");

}

void printReg(char registoDeAtividade[SIZEARRAY][90])
{
        int i;
        printf("   ├─[Registo de Atividade]──────────────────────────────────────────────────┤\n");
        for (i = 0; i < SIZEARRAY; i++)
        {
                char * pntThroughPnt = registoDeAtividade[i];

                if(strcmp(pntThroughPnt, "0") != 0)printf("%s\n", pntThroughPnt);
                else printf("   │                                                                         │\n");

        }
        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");
}
void printRod()
{
        printf("   │      [C]-Começar simulação                 [T]-Terminar simulação       │\n");
        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");
        printf("   │                         Projecto Realizado por:                         │\n");
        printf("   │             •André Nunes •Diogo Coelho •Susana Gonçalves                │\n");
        printf("   │               (2029315)     (2030515)     (2027615)                     │\n");
        printf("   └─────────────────────────────────────────────────────────────────────────┘\n");

}

void ecra(int sockfd)
{
        struct estatisticas stats = {100000, 10, 100, 0, 0, 0};
        char registoDeAtividade[SIZEARRAY][90];
        int i; for(i = 0; i < SIZEARRAY; i++){strcpy(registoDeAtividade[i], "0");}
        char line[MAXLINE];
        int n;
        int nArray = 0;
        char sendline[MAXLINE];

        for(;;) {
        if((n = read(sockfd, line, MAXLINE-1)) > 0)
        {
                //é preciso porque partimos a string duas vezes, e não podemos fazer no mesmo apontador
                char * auxLine = (char *) malloc(sizeof(char) * 3); //aloca memoria para auxLine
                strcpy(auxLine, line); // copia a line para a auxLine

                stats = atualizaDadosEstatisticas(line, ";", stats); //atualiza a estrutura consoante a mensagem recebida

                if(nArray <= SIZEARRAY - 1)
                {
                        strcpy( registoDeAtividade[nArray], protocologoComunicacao(auxLine, ";"));
                        nArray++;
                }
                else
                {
                        char * temp;
                        int i;
                        for(i = 0; i < SIZEARRAY; i++)
                        {
                                strcpy(registoDeAtividade[i], registoDeAtividade[i+1]);
                        }
                        strcpy( registoDeAtividade[SIZEARRAY-1], protocologoComunicacao(auxLine, ";"));
                }

                bzero(line,MAXLINE);

                printCab();
                printStats(stats);
                printReg(registoDeAtividade);
                printRod();

                int i;
        }

        else if(n<0) printf("ERRO LENDO DA SOCKFD");
        }

}
