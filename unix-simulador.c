
/* Servidor do tipo socket stream.
   Recebe linhas do cliente e reenvia-as para o cliente */

#include "unix.h"

void main(void)
{

	int sockfd, newsockfd, clilen, childpid, servlen;
	struct sockaddr_un cli_addr, serv_addr;

	/* Cria socket stream */

	if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
		err_dump("server: can't open stream socket");

	/* Primeiro uma limpeza preventiva!
	   Dados para o socket stream: tipo + nome do ficheiro.
         O ficheiro serve para que os clientes possam identificar o servidor */

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, UNIXSTR_PATH);

      /* O servidor � quem cria o ficheiro que identifica o socket.
         Elimina o ficheiro, para o caso de algo ter ficado pendurado.
         Em seguida associa o socket ao ficheiro.
         A dimens�o a indicar ao bind n�o � a da estrutura, pois depende
         do nome do ficheiro */

	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
	unlink(UNIXSTR_PATH);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
		err_dump("server, can't bind local address");

	/* Servidor pronto a aceitar 5 clientes para o socket stream */

	listen(sockfd, 1);

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) err_dump("server: accept error");


        printf("Simulador\n");
        for (;;) {
                close(sockfd);
                montanha_russa(newsockfd);
                exit(0);
        }
        close(newsockfd);

}
