#include "unix.h"

#define MAXLINE 512

int newsockfd;

void * threadRoutine(newsockfd)
{
        char lineToWrite[50];
        sprintf(lineToWrite, "Olá, sou o cliente %i", pthread_self());
        printf("%s\n",lineToWrite);
}
void tempo_aberto_parque(){

        simulador.tempo_aberto = (simulador.hora_de_fecho-simulador.hora_de_abertura) *60;
        printf("%d", simulador.tempo_aberto);
}
void * virtualtime() // função que incrementa variavel do tempo virtual
{
        while (simulador.aberto) {
                //gettime(auxtime);
                usleep(150000);
                simulador.contador_time++;
                gettime();

        }
}
void gettime( ) { // funçao para obter o tempo virtual

        int horas = simulador.hora_de_abertura;
        int min;

        horas = horas + simulador.contador_time/60;
        min = simulador.contador_time % 60;

        printf ("São %dh%dmin\n",horas, min);
}
void timersimulador() // cria a thread para a contagem do tempo virtual
{
        pthread_t thread;
        if((pthread_create(&(thread),NULL,(void *)virtualtime, NULL))!=0)
        {
                err_dump("pthread_create: erro criação thread");
        }
        else printf("criou timer\n" );
}
void *divertimento(){
        while(simulador.aberto) {
                if (simulador.contador_time >= simulador.tempo_aberto) {
                        simulador.divertimento_boolean = 0;
                }
        }
}

void *fury325(int socket){
        while(simulador.divertimento_boolean) {
                printf("FURY 325 está pronto a receber clientes\n");
                int pessoas_fury = 0;
                int pessoas_verificadas = 0;
                while (pessoas_fury < 4) {
                        sem_wait(&s_fury325); // o fury começa a trabalhar quando tiver pelo menos 4 pessoas
                        pessoas_fury++;

                }
                if(simulador.tempo_aberto > simulador.contador_time ) {
                        pthread_mutex_lock(&mutex_fury);


                        for (int i = 0; i < pessoas_fury; i++) {
                                num_pessoas_fury325--;
                                sem_post(&s_cliente_fury325); // da assinala que existe 4 lugares disponiveis
                        }
                        pthread_mutex_unlock(&mutex_fury);
                        for (int i = 0; i < 4; i++) {
                          sem_post(&s_cliente_tempo);
                        }
                        while(pessoas_verificadas < 4) {
                          sem_wait(&s_cliente_verificado);
                          pessoas_verificadas++;
                        }


                        //sleep (1);
                        printf("FURY 325 saiu\n");
                        sleep(4); // tempo da volta da montanhavoid * fury325(int socket)
                        printf("FURY 325 ACABOU \n");
                        for ( int i = 0; i < pessoas_fury; i++) {
                                sem_post(&s_finish_fury); // assinala que acabou a volta
                                //sem_wait(&s_comunicacaofury325);
                        }
                }


        }

      //  printf("FURY325 fechou as portas.\n" );
        for ( int i = 0; i < num_pessoas_fury325; i++) {
                sem_post(&s_cliente_fury325);

                //sem_post(&s_finish_fury); // assinala que acabou a volta

        }

}



void *cliente_fury(int id){

        pthread_mutex_lock(&mutex_fury);
        num_pessoas_fury325++;
        sem_post(&s_fury325);
        // avisa ao fury que tem clientes
        pthread_mutex_unlock(&mutex_fury);
        sem_wait(&s_cliente_tempo);
        if (simulador.contador_time < simulador.tempo_aberto -20 && cliente[id].tempo_max_espera > (simulador.contador_time - cliente[id].tempo_chegou_divertimento)) {
        sem_wait(&s_cliente_fury325);         // retira os cliente da fila (estão dentro do fury)


                printf("O Cliente n%d entrou no FURY 325\n",id);
                sem_post(&s_cliente_verificado);
                sem_wait(&s_finish_fury); // retira os clientes do fury
                pthread_mutex_lock(&mutex_comunicacao);
                //sem_post(&s_comunicacaofury325);
                printf("O Cliente n%d saiu do FURY 325\n", id);
                pthread_mutex_unlock(&mutex_comunicacao);


        }
        else{
                sem_post(&s_cliente_tempo);
                num_pessoas_fury325--;
                printf("Cliente %d desistiu de andar no fury .\n", id);
        }
}


void * takabisha (int socket){
        while (simulador.divertimento_boolean) {
                //int pessoas_para_entrar = 0;
                printf("TAKABISHA preparado para andar.\n" );
                //  while (pessoas_para_entrar < 2 ) {
                sem_wait(&s_takabisha);
                if(simulador.tempo_aberto > simulador.contador_time ) {
                        //    pessoas_para_entrar++;
                        //  }
                        pthread_mutex_lock(&mutex_takabisha);

                        //  printf("TAKABISHA fechou as portas \n")  for (int i = 0; i < pessoas_para_entrar; i++) {
                        if (num_prio_takabisha>= 1)
                        {
                                num_prio_takabisha--;
                                sem_post(&s_prio_takabisha);

                        }
                        else{
                                num_sem_prio_takabisha--;
                                sem_post(&s_sem_prio_takabisha);
                        }
                        //  }
                        pthread_mutex_unlock(&mutex_takabisha);

                        sem_wait(&s_sai_takabisha);
                      // tempo da volta da montanha
                        printf("TAKABISHA SAIU \n");
                        sleep(3);
                        printf("TAKABISHA ACABOU\n");
                        //for ( int i = 0; i < pessoas_para_entrar; i++) {
                        sem_post(&s_finish_takabisha);
                        sem_wait(&s_comunicacao_takabisha);
                        //}
                }
        }

        for ( int i = 0; i < num_prio_takabisha; i++) {
          sem_post(&s_prio_takabisha);
          sem_post(&s_finish_takabisha);
        }
        for (int i = 0; i < num_sem_prio_takabisha; i++) {
          sem_post(&s_sem_prio_takabisha);
          sem_post(&s_finish_takabisha);
        }


}

void *cliente_takabisha(int id){
        pthread_mutex_lock(&mutex_takabisha);
        if (cliente[id].prioridade == 1) {
                num_prio_takabisha++;
        }
        else{
                num_sem_prio_takabisha++;
        }
        sem_post(&s_takabisha);

        if (cliente[id].prioridade == 1) { // cliente que tem prioridade
                pthread_mutex_unlock(&mutex_takabisha);
                sem_wait(&s_prio_takabisha);
                if(simulador.divertimento_boolean){
                printf("Entrou cliente %d prioritario no carro \n", id );
                sem_post(&s_sai_takabisha);
                }

        }
        else{ // cliente sem prioridade
                pthread_mutex_unlock(&mutex_takabisha);
                sem_wait(&s_sem_prio_takabisha);
                if(simulador.divertimento_boolean){
                printf("Entrou cliente %d nao prioritario no carro \n", id );
                sem_post(&s_sai_takabisha);
              }
        }

        sem_wait(&s_finish_takabisha);
        printf("O cliente %d saiu do TAKABISHA\n", id);
        sem_post(&s_comunicacao_takabisha);
}


void *escolhedivertimento(int id){
        switch (cliente[id].divertimento) {
        case 0:
                cliente[id].tempo_chegou_divertimento = simulador.contador_time;
                cliente_takabisha(id);
                //printf("cliente cheogu aqui crl\n" );
                break;

        case 1:
                cliente[id].tempo_chegou_divertimento = simulador.contador_time;
                cliente_fury(id);

                break;

        default: printf("Erro na escolha do divertimento");
        }
}


void *bilheteira()
{
        while(simulador.aberto) {
                sem_wait(&s_prod_bilheteira);
                pthread_mutex_lock(&mutex_bilheteira);
                if((rand()%100)< simulador.perc_prioridade) // define se o cliente é prioritário ou não
                {simulador.buff_bilheteira[produz_bilheteira] = 1; }
                else
                {simulador.buff_bilheteira[produz_bilheteira] = 2; }
                //printf("%d\n",simulador.buff_bilheteira[produz_bilheteira] );
                //printf("%d\n", produz_bilheteira);
                produz_bilheteira = ( produz_bilheteira + 1 )% 4;
                pthread_mutex_unlock(&mutex_bilheteira);
                sem_post(&s_cons_bilheteira);
        }


}

void bilhete(int id)
{
        //printf("COMPROU BILHETE ESTE FDP");
        if(simulador.aberto) {
                printf("%d CONSEGUI CHEGAR AQUI \n", id);
                sem_wait(&s_cons_bilheteira);
                pthread_mutex_lock(&mutex_bilheteira);
                if (simulador.buff_bilheteira[consome_bilheteira] == 1)
                {
                        cliente[id].prioridade = 1;
                        //printf("definiu prioridade1");
                }
                else if(simulador.buff_bilheteira[consome_bilheteira]== 2)
                {
                        cliente[id].prioridade = 0;
                        //printf("definiu prioridade0");
                }
                simulador.buff_bilheteira[consome_bilheteira]= 0;
                cliente[id].divertimento = rand()%2;
                printf("O cliente n%d tem o valor de prioridade %d e o divertimento é %d\n",id, cliente[id].prioridade,cliente[id].divertimento);

                consome_bilheteira = (consome_bilheteira + 1)% 4;
                pthread_mutex_unlock(&mutex_bilheteira);
                sem_post(&s_prod_bilheteira);
                //printf ("MERDA PO CRL COM ISTO TUDO \n");
        }
        else{
        printf("O Cliente %d saiu do parque, pois este fechou\n",id );
        }
}
void vai_bilheteira(int id){
        cliente[id].bilheteira = rand()%100;
        /*while (cliente[id].bilheteira > simulador.perc_cliente_bilhete) { // ciclo para fazer os cliente perder tempo para irem para a bilhetaria !
                cliente[id].bilheteira = rand()%100;
                printf("O cliente n%d está a espera.\n", id);
           }*/
        sleep(3);
        if (cliente[id].bilheteira < simulador.perc_cliente_bilhete) {
                printf("O cliente n%d ta na bilheteira.\n",id);
                if(simulador.divertimento_boolean){
                bilhete(id);
                }
                else{
                  printf("A bilheteira está fechada.\n");
                }
        }

}

void *trata_cliente(int id)
{ //para usar o rand()

        time_t t;
        srand((unsigned) time(&t));
        cliente[id].tempo_max_espera = rand()%20;
        if(cliente[id].tempo_max_espera < 10) { // faz o tempo variar apenas entre 10 e 20
                cliente[id].tempo_max_espera = cliente[id].tempo_max_espera +10;
        }
        /*gettime( simulador.contador_time);
           char lineBilheteria [50];
           sprintf(lineBilheteria, "%d;2;a", id);*/
        printf(", O Cliente nº%d chegou ao parque.\n",id);

        //write(newsockfd, lineBilheteria, strlen(lineBilheteria));

        sem_wait(&s_tam_max_parque);     // fazer post disto, ainda estou a pensar onde

        printf(" O Cliente nº%d entrou no parque.\n",id);
        //sem_wait(&s_tam_fila_bilheteira);// em principio não vamos precisar deste semaforo
        vai_bilheteira(id);
        escolhedivertimento(id);
        //METER SEMAFORO DO TAMANHO tam_fila_bilheteiraDA FILA DE ESPERA PARA ENTRAR NO PARQUE




        // fazer isto na bilheteira ->cliente[id].divertimento = rand()%3;

        /*  if(cliente[id].prioridade == 1)
           {
                  gettime(simulador.contador_time);
                  printf(", O Cliente nº%d, sem prioridade, entrou no parque.\n",id);
           }
           else
           {
                  gettime(simulador.contador_time);
                  printf(", O Cliente nº%d,com prioridade, entrou no parque.\n",id);
           }*/

        //  sem_post(&s_tam_fila_bilheteira);

        printf("O Cliente %d saiu do parque\n", id );
        sem_post(&s_tam_max_parque);

}
void *cria_cliente(int socket)
{
        newsockfd = socket;
        for (int i = 0; i < simulador.max_cliente; i++) {
                usleep(150000);
                usleep(150000);
                usleep(150000);



                if((pthread_create(&(t_cliente[i]),NULL,(void *)trata_cliente, i))!=0)
                {
                        err_dump("pthread_create: erro criação thread");
                }
                else{
                            conta_cliente++;
                           char lineCriacao [50];
                           sprintf(lineCriacao, "%d;1;a", i);
                           printf("O Cliente nº%d chegou ao parque.\n",i);
                           write(newsockfd, lineCriacao, strlen(lineCriacao));
                }

        }
}
void *cria_bilheteira(int socket)
{
        int i = 0;
        while (simulador.aberto) {
                // cria bilheteira enquanto for menor que o numero max de bilheteiras


                if((pthread_create(&(t_bilheteira[i]),NULL,(void *)bilheteira,NULL))!=0)
                {
                        err_dump("pthread_create: erro criação thread");
                }
                else
                {
                        i++;
                        conta_bilheteira++;
                        printf("criou bilheteira");
                }
                usleep(150000 * (rand()%100));

        }
}
