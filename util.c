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
		case 1: acao = "entrou";
		break;
		case 2: acao = "esta na fila para";
		break;
		case 3: acao = "já não se encontra";
                break;
                case 4: acao = "desistiu de entrar";
		break;
		default: acao = "ERRO!";
	}

	char * lugar;
	switch (*strtok(NULL, separador)) {
		case 'a': lugar = "no parque";
		break;
		case 'b': lugar = "na bilhetaria";
		break;
		case 'c': lugar = "na montanha russa 1";
		break;
		case 'd': lugar = "na montanha russa 2";
		break;
		case 'e': lugar = "na montanha russa 2";
		break;
		default: lugar = "ERRO!";
	}

	char * fraseCompleta = (char *) malloc(sizeof(char) * 3); //Não sei bem, mas a internet disse e funciona!
	sprintf(fraseCompleta, "O cliente %s %s %s.", idCliente, acao, lugar);
	return fraseCompleta;
 }
