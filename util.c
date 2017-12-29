#include "unix.h"
#include <stdio.h>
#include <errno.h>

/* L� nbytes de um ficheiro/socket.
   Bloqueia at� conseguir ler os nbytes ou dar erro */
int readn(int fd, char *ptr, int nbytes)
{
        int nleft, nread;

        nleft = nbytes;
        while (nleft > 0) {
                nread = read (fd, ptr, nleft);
                if (nread < 0)
                        return (nread);
                else if (nread == 0)
                        break;

                nleft -= nread;
                ptr += nread;
        }
        return (nbytes - nleft);
}

/* Escreve nbytes num ficheiro/socket.
   Bloqueia at� conseguir escrever os nbytes ou dar erro */
int writen(int fd, char *ptr, int nbytes)
{
        int nleft, nwritten;

        nleft = nbytes;
        while (nleft > 0) {
                nwritten = write(fd, ptr, nleft);
                if (nwritten <= 0)
                        return(nwritten);

                nleft -= nwritten;
                ptr += nwritten;
        }
        return(nbytes - nleft);
}

/* L� uma linha de um ficheiro/socket (at� \n, maxlen ou \0).
   Bloqueia at� ler a linha ou dar erro.
   Retorna quantos caracteres conseguiu ler */
int readline(int fd, char *ptr, int maxlen)
{
        int n, rc;
        char c;

        for (n=1; n<maxlen; n++) {
                if ((rc = read(fd, &c, 1)) == 1) {
                        *ptr++ = c;
                        if (c == '\n')
                                break;
                } else if (rc == 0) {
                        if (n == 1)
                                return(0);
                        else
                                break;
                } else
                        return (-1);
        }

        /* N�o esquecer de terminar a string */

        *ptr = 0;

        /* Note-se que n foi incrementado de modo a contar
           com o \n ou \0 */

        return (n);
}

/* Mensagem de erro */
void err_dump(char *msg)
{
        perror(msg);
        exit(1);
}

char * protocologoComunicacao(char * codigo, char * separador)
{
        int *idCliente = strtok(codigo, separador);

        char * acao;
        switch (strtol(strtok(NULL, separador),NULL,0)) { //strtol - converte char para inteiro
        case 1: acao = "chegou";
                break;
        case 2: acao = "entrar";
                break;
        case 3: acao = "esta na fila para comprar";
                break;
        case 4: acao = "esta na fila para entrar";
             break;
        case 5: acao = "já não se encontra";
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
        case 'd': lugar = "na montanha russa Takabisha";
                break;
        case 'e': lugar = "na montanha russa Fury 325";
                break;
        default: lugar = "ERRO!";
        }

        char * fraseCompleta = (char *) malloc(sizeof(char) * 3); //Não sei bem, mas a internet disse e funciona!

        sprintf(fraseCompleta, "   │%s • O cliente %s %s %s.", "10h00", idCliente, acao, lugar); //cooloca informação pretinente

        int aux = strlen(fraseCompleta);
        aux = 79 - aux;
        sprintf(fraseCompleta, "%s %*s │", fraseCompleta, aux, " "); //adiciona os espaços vazios para enquadrar no ecrã

        return fraseCompleta;
}

struct estatisticas atualizaDadosEstatisticas(char * codigo, char * separador, struct estatisticas stats)
{
        strtok(codigo, separador); //id do cliente, não é necessário

        switch (strtol(strtok(NULL, separador),NULL,0)) {  //strtol - converte char para inteiro
        case 1:          //entrou
                switch (*strtok(NULL, separador)) {
                case 'a':
                        break;
                case 'b':
                        stats.total_parque++;
                        break;
                case 'c': stats.total_bilhetes_vendidos++;
                        break;
                case 'd': stats.total_montanha1++;
                        break;
                case 'e': stats.total_montanha2++;
                        break;
                }
                break;
        case 4:         //desistiu
                stats.tempo_desistencias += 10;
                stats.total_desistencias++;
                break;
        }

        return stats;

}

void escreveEstatisticasFicheiro(struct estatisticas stats)
{
        FILE *file;

        file = fopen("estatisticas", "w+"); //w+ - abre ficheiro, se não existe cria
        fprintf(file, "ESTATISTICAS\n");
        fprintf(file, "Número de pessoas que entrou no parque nesta simulação: %d pessoas\n ", stats.total_parque);
        fprintf(file, "Número de bilhetes vendidos nesta simulação: %d bilhetes\n ", stats.total_bilhetes_vendidos);
        fprintf(file, "Número de pessoas que andou na montanha russa Fury 325: %d pessoas\n ", stats.total_montanha1);
        fprintf(file, "Número de pessoas que andou na montanha russa Takabisha: %d pessoas\n ", stats.total_montanha2);
        fprintf(file, "Número de pessoas que desistiu enquanto estava na filha: %d pessoas\n ", stats.total_desistencias);
        fprintf(file, "Tempo media antes de desistencia: %d mins\n ", (stats.tempo_desistencias/stats.total_desistencias));

        fclose(file);
}
