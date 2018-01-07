#include "unix.h"
#define MAXLINE 512

int estado = 0;
struct estatisticas stats;
//ESTOU AQUI

char * protocologoComunicacao(char * codigo, char * separador)
{
        char * hora = strtok(codigo, separador);

        char * idCliente = strtok(NULL, separador);

        char * acao;
        switch (strtol(strtok(NULL, separador),NULL,0)) { //strtol - converte char para inteiro
        case 1: acao = "chegou";
                break;
        case 2: acao = "entrou";
                break;
        case 3: acao = "esta na fila para comprar";
                break;
        case 4: acao = "esta na fila para entrar";
                break;
        case 5: acao = "ja nao se encontra";
                break;
        case 6: acao = "desistiu de entrar";
                break;
        default: acao = "ERRO!";
        }

        char * lugar;
        switch (*strtok(NULL, separador)) {
        case 'a': lugar = "ao recinto do parque";
                break;
        case 'b': lugar = "no recinto do parque";
                break;
        case 'c': lugar = "na bilhetaria";
                break;
        case 'd': lugar = "na montanha Takabisha";
                break;
        case 'e': lugar = "na montanha Fury 325";
                break;
        default: lugar = "ERRO!";
        }

        char * fraseCompleta = (char *) malloc(sizeof(char) * 3); //Não sei bem, mas a internet disse e funciona!

        sprintf(fraseCompleta, "   │%s • O cliente %s %s %s. ", hora, idCliente, acao, lugar); //cooloca informação pretinente

        int aux = strlen(fraseCompleta);
        aux = 80 - aux;

        sprintf(fraseCompleta, "%s %*s│", fraseCompleta, aux, " "); //adiciona os espaços vazios para enquadrar no ecrã

        return fraseCompleta;
}

void atualizaDadosEstatisticas(char * codigo, char * separador)
{
        strtok(codigo, separador); //hora, não é necessário
        strtok(NULL, separador);//id do cliente, não é necessário

        switch (strtol(strtok(NULL, separador),NULL,0))
        {
                case 1: //chegou
                        break;
                case 2: //entrou
                        switch (*strtok(NULL, separador))
                        {
                                case 'a': //ao recinto do parque
                                        break;
                                case 'b': //no recinto do parque
                                        stats.total_parque++;
                                        break;
                                case 'c': // na bilhetaria
                                        break;
                                case 'd'://na montanha russa Takabisha
                                        stats.total_montanha1++;
                                        break;
                                case 'e': //na montanha Fury 325
                                        break;
                        }
                        break;
                case 3: //esta na fila para comprar
                        break;
                case 4: //esta na fila para entrar
                        break;
                case 5: //ja não se encontra
                        switch (*strtok(NULL, separador))
                        {
                                case 'c': //na bilhetaria
                                        stats.total_bilhetes_vendidos++;
                                        break;
                                case 'e': //na montanha Fury 325
                                        stats.total_montanha2++;
                                        break;
                        }
                        break;
                case 6: //desistiu de entrar
                        stats.tempo_desistencias += 10;
                        stats.total_desistencias++;
                        break;
        }
}

void escreveEstatisticasFicheiro()
{
        FILE * file = fopen("estatisticas.txt", "w"); //w+ - abre ficheiro, se não existe cria

        fprintf(file, "┌──────────────────────────────────ESTATISTICAS───────────────────────────\n");
        fprintf(file, "│ • Número de pessoas que entrou no parque nesta simulação: %d pessoas\n", stats.total_parque);
        fprintf(file, "│ • Número de bilhetes vendidos nesta simulação: %d bilhetes\n", stats.total_bilhetes_vendidos);
        fprintf(file, "│ • Número de pessoas que andou na montanha russa Fury 325: %d pessoas\n", stats.total_montanha1);
        fprintf(file, "│ • Número de pessoas que andou na montanha russa Takabisha: %d pessoas\n", stats.total_montanha2);
        fprintf(file, "│ • Número de pessoas que desistiu enquanto estava na fila: %d pessoas\n", stats.total_desistencias);
        //fprintf(file, "│ • Tempo media antes de desistencia: %d mins\n ", (stats.tempo_desistencias/stats.total_desistencias));

        //fclose(file);
}

int acertaStatsN(int stat)
{
        if (stat == 0) return 1;
        else return floor (log10 (abs (stat))) + 1;
}

void printCab()
{
        if (estado == 0)
        {
                printf("   ┌─────────────────────────█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█──────────────────────────┐\n");
                printf("   │                         █░░╦ ╦╔╗╦ ╔╗╔╗╔╦╗╔╗░░█                          │\n");
                printf("   │                         █░░║║║╠ ║ ║ ║║║║║╠ ░░█                          │\n");
                printf("   │                         █░░╚╩╝╚╝╚╝╚╝╚╝╩ ╩╚╝░░█                          │\n");
                printf("   ├─────────────────────────█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█──────────────────────────┤\n");
        }
        else
        {
                printf("   ┌─────────────────────────────────────────────────────────────────────────┐\n");
                printf("   │                          DISNEYLAND MADEIRA                             │\n");
                printf("   │                    *Sistemas Operativos 2017/2018*                      │\n");
                printf("   │                        Universidade da Madeira                          │\n");
                printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");
        }

}

void printStats()
{


        printf("   ├─[Estatísticas]──────────────────────────────────────────────────────────┤\n");
        if(estado == 0)
        {
                printf("   │                                                                         │\n");
                printf("   │                                                                         │\n");
                printf("   │                                                                         │\n");
                printf("   │                                                                         │\n");
                printf("   │                                                                         │\n");

        }
        else
        {
                int aux;

                if(acertaStatsN(stats.total_parque) <= 20) //confirma que não passa do limite superado pela pagina
                {
                        //calcula o espaço vazio em seguida ao números
                        if(21-acertaStatsN(stats.total_parque) < 0) aux = 0;
                        else aux = 21-acertaStatsN(stats.total_parque);
                        printf("   │• Numero total de pesssoas no parque hoje: %d pessoas %*s│\n",stats.total_parque, aux, " ");
                }
                else //caso número seja muito grande, faz print do maior número superado
                {
                        printf("   │• Numero total de pesssoas no parque hoje: +99999999999999999999 pessoas │\n");
                }

                if(acertaStatsN(stats.total_bilhetes_vendidos) <= 31)
                {
                        if(32-acertaStatsN(stats.total_bilhetes_vendidos) < 0) aux = 0;
                        else aux = 32-acertaStatsN(stats.total_bilhetes_vendidos);
                        printf("   │• Numero de bilhetes vendidos: %d bilhetes %*s│\n", stats.total_bilhetes_vendidos, aux, " ");
                }
                else
                {
                        printf("   │• Numero de bilhetes vendidos: +9999999999999999999999999999999 pessoas │\n");
                }

                if(acertaStatsN(stats.total_montanha1) <= 4)
                {
                        if(5-acertaStatsN(stats.total_montanha1) < 0) aux = 0;
                        else aux = 5-acertaStatsN(stats.total_montanha1);
                        printf("   │• Numero de pessoas que andou na montanha russa Takabisha: %d pessoas %*s│\n",stats.total_montanha1, aux, " ");
                }
                else
                {
                        printf("   │• Numero de pessoas que andou na montanha russa Fury 325: +9999 pessoas │\n");
                }

                if(acertaStatsN(stats.total_montanha2) <= 5)
                {
                        if(6-acertaStatsN(stats.total_montanha2) < 0) aux = 0;
                        else aux = 6-acertaStatsN(stats.total_montanha2);
                        printf("   │• Numero de pessoas que andou na montanha russa Fury 325: %d pessoas %*s│\n",stats.total_montanha2, aux, " ");
                }
                else
                {
                        printf("   │• Numero de pessoas que andou na montanha russa Fury 325: +99999 pessoas │\n");

                }

                if(acertaStatsN(stats.total_desistencias) <= 32)
                {
                        if(33-acertaStatsN(stats.total_desistencias) < 0) aux = 0;
                        else aux = 33-acertaStatsN(stats.total_desistencias);
                        printf("   │• Numero total de desistentes: %d pessoas %*s│\n",stats.total_desistencias, aux, " ");
                }
                else
                {
                        printf("   │• Numero total de desistentes: +99999999999999999999999999999999 pessoas │\n");
                }
        }

        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");

}

void printReg(char registoDeAtividade[SIZEARRAY][90])
{
        int i;
        printf("   ├─[Registo de Atividade]──────────────────────────────────────────────────┤\n");
        for (i = 0; i < SIZEARRAY; i++)
        {
                char * pntThroughPnt = registoDeAtividade[i];

                if(strcmp(pntThroughPnt, "0") != 0) printf("%s\n", pntThroughPnt);
                else printf("   │                                                                         │\n");

        }
        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");
}

void printRod()
{
        if(estado == 0) printf("   │                         [C]-Começar simulação                           │\n");
        else if(estado == 1) printf("   │                          Simulação à decorrer                           │\n");
        else if(estado == 2) printf("   │                          Simulação terminada                            │\n");
        printf("   ├─────────────────────────────────────────────────────────────────────────┤\n");
        printf("   │                         Projecto Realizado por:                         │\n");
        printf("   │              •André Nunes •Diogo Coelho •Susana Gonçalves               │\n");
        printf("   │                (2029315)     (2030515)     (2027615)                    │\n");
        printf("   └─────────────────────────────────────────────────────────────────────────┘\n");



}

void ecra(int sockfd)
{
        stats.total_parque = 0;
        char registoDeAtividade[SIZEARRAY][90];
        int nArray = 0;
        for(int i = 0; i < SIZEARRAY; i++) {strcpy(registoDeAtividade[i], "0");} //inicialização do array registoDeAtividade[][]

        char line[MAXLINE];
        char key;
        int n;

        //aparecer a primeira vez a interface
        printCab();
        printStats();
        printReg(registoDeAtividade);
        printRod();

        for(;;)
        {
                if(estado == 0)
                {
                        int key = getchar();
                        if(key== 99 || key == 67) // 99 - c(minusculo); 67 - C(maiusculo)
                        {
                                estado = 1;

                                write(sockfd, "c", MAXLINE);

                                printCab();
                                printStats();
                                printReg(registoDeAtividade);
                                printRod();
                        }
                }
                else if(estado == 1)
                {
                        if((n = read(sockfd, line, MAXLINE-1)) > 0)
                        {
                                if(strcmp(line, "end") == 0)
                                {
                                        estado = 2;
                                }
                                else
                                {
                                        //é preciso porque partimos a string duas vezes, e não podemos fazer no mesmo apontador
                                        char * auxLine = (char *) malloc(sizeof(char) * 3); //aloca memoria para auxLine
                                        strcpy(auxLine, line); // copia a line para a auxLine

                                        atualizaDadosEstatisticas(line, ";");

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
                                        printStats();
                                        printReg(registoDeAtividade);
                                        printRod();

                                        write(sockfd, "q", MAXLINE);
                                }
                        }

                        else if(n<0) printf("ERRO LENDO DA SOCKFD");
                }else if(estado == 2)
                {
                        printCab();
                        printStats();
                        printReg(registoDeAtividade);
                        printRod();

                        escreveEstatisticasFicheiro();
                        write(sockfd, "q", MAXLINE);
                        break;
                }
        }
}
