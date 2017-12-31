
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

								/* Servidor pronto a aceitar 1 clientes para o socket stream */

								listen(sockfd, 1);

								clilen = sizeof(cli_addr);
								newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

								if (newsockfd < 0) err_dump("server: accept error");

								simulador.perc_prioridade = 12;
								simulador.max_cliente = 72;
								simulador.contador_time = 0;
								simulador.aberto = 1;
								simulador.perc_cliente_bilhete= 100;
								simulador.buff_bilheteira[4];
								simulador.hora_de_abertura= 8;
								simulador.hora_de_fecho = 14;
								simulador.divertimento_boolean = 1;


								//sem_init(&s_tam_fila_bilheteira,0, 10);
								sem_init(&s_tam_max_parque,0, simulador.max_cliente);
								sem_init(&s_prod_bilheteira,0, 4);
								sem_init(&s_cons_bilheteira,0, 0);
								sem_init(&s_prio_takabisha,0, 0);
								sem_init(&s_sem_prio_takabisha,0, 0);
								sem_init(&s_finish_takabisha,0, 0);
								sem_init(&s_fury325,0, 0);
								sem_init(&s_cliente_fury325,0,0);
								sem_init(&s_finish_fury,0,0);
								sem_init(&s_comunicacaofury325,0,0);
								sem_init(&s_sai_takabisha,0,0);
								sem_init(&s_cliente_verificado,0,0);


								pthread_mutex_init(&mutex_fury,NULL);
								pthread_mutex_init(&mutex_takabisha,NULL);
								pthread_mutex_init(&mutex_bilheteira,NULL);
								pthread_mutex_init(&mutex_comunicacao,NULL);
								produz_bilheteira = 0;
								verifica_cliente_takabisha = 0;
								consome_bilheteira = 0;

								tempo_aberto_parque();

								if((pthread_create(&(thread),NULL,(void *)virtualtime, NULL))!=0)
				        {
				                err_dump("pthread_create: erro criação thread");
				        }
				        else printf("criou timer\n" );

								if((pthread_create(&(t_cria_booleano_divertimento),NULL,(void *)divertimento,NULL))!=0){
										err_dump("pthread_create: erro criação thread");
								}
								else{

																printf("criou booleano dos divertimentos\n");
								}
								if((pthread_create(&(t_cria_fury),NULL,(void *)fury325, NULL))!=0){

											err_dump("pthread_create: erro criação thread");
								}
								else{

																printf("criou fury325\n");
								}
								if((pthread_create(&(t_cria_takabisha),NULL,(void *)takabisha, NULL))!=0){

											err_dump("pthread_create: erro criação thread");
								}
								else{

																printf("criou takabisha\n");
								}
								if((pthread_create(&(t_cria_cliente),NULL,(void *)cria_cliente,newsockfd))!=0)
								{
																err_dump("pthread_create: erro criação thread");
								}
								else{

																printf("%d", conta_cliente);
								}
								if((pthread_create(&(t_cria_bilheteira),NULL,(void *)cria_bilheteira, NULL))!=0)
								{
																err_dump("pthread_create: erro criação thread");
								}
								else{

								}

								/*usleep(150000);
								usleep(150000);
								usleep(150000);
								usleep(150000);
								usleep(150000);*/
								printf("apasssss %d", conta_cliente);
								int j= 0;
								while(conta_cliente < simulador.max_cliente);

								for (int i = 0; i < simulador.max_cliente; i++) {

																pthread_join(t_cliente[i], NULL);
																usleep(150000);
																j++;
																printf("%d",j);
								}
								pthread_join(t_cria_cliente, NULL);
								
								usleep(1500000);

								simulador.aberto = 0 ;
								printf("PUTA CHEGUEI AQUI\n");
								//pthread_join(t_cria_takabisha, NULL);
								printf("CRL O TAKABISHA NAO ACABOU\n");
								//pthread_join(t_cria_fury, NULL);

								/*for(int j = 0; j < num_bilheteiras; j++)
								{
																pthread_join(t_bilheteira[j], NULL);
								}*/
								printf("FURY325 fechou as portas.\n" );
								printf("TAKABISHA fechou as portas \n");
								printf("O PARQUE FECHOU !!\n");



								close(sockfd); //cuidado com isto senão não deixa acabar as threads


}
