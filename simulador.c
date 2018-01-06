#include "unix.h"

#define MAXLINE 512

int newsockfd;

void leConfig()
{
        char parametro[MAXLINE];
        int valor;
        FILE * file = fopen("config.txt", "r");

        file = fopen("config.txt", "r");

        //valor predefinidos para o caso de não abrir o ficheiro ou faltar parametros.
        simulador.perc_prioridade = 10;
        simulador.max_cliente = 1000;
        simulador.clientes_criados = 2000;
        simulador.contador_time = 0;
        simulador.aberto = 1;
        simulador.perc_cliente_bilhete = 100;
        simulador.hora_de_abertura= 8;
        simulador.hora_de_fecho = 12;
        simulador.divertimento_boolean = 1;

        if(file == NULL)
        {
                printf("%s\n", "│    ★ Erro ao abrir o ficheiro. Serão usados os valores de configuração predefinidos.");
        }
        else
        {
                printf("│    ★ À ler valores de configuração.\n");

                while(fscanf(file, "%s : %d\n", parametro, &valor)!=EOF) /* read a line */
                {
                        if(strcmp(parametro, "perc_prioridade")==0) simulador.perc_prioridade = valor;
                        else if(strcmp(parametro, "max_cliente")==0) simulador.max_cliente = valor;
                        else if(strcmp(parametro, "clientes_criados")==0) simulador.clientes_criados = valor;
                        else if(strcmp(parametro, "contador_time")==0) simulador.contador_time = valor;
                        else if(strcmp(parametro, "aberto")==0) simulador.aberto = valor;
                        else if(strcmp(parametro, "perc_cliente_bilhete")==0) simulador.perc_cliente_bilhete = valor;
                        else if(strcmp(parametro, "hora_de_abertura")==0) simulador.hora_de_abertura = valor;
                        else if(strcmp(parametro, "hora_de_fecho")==0) simulador.hora_de_fecho = valor;
                        else if(strcmp(parametro, "divertimento_boolean")==0) simulador.divertimento_boolean = valor;
                        else printf(" │    ★ Erro a ler parametro do ficheiro.\n");

                }
        }

        fclose(file);
        printf("│    ★ À iniciar simulação.\n");
}

void tempo_aberto_parque()
{

        simulador.tempo_aberto = (simulador.hora_de_fecho-simulador.hora_de_abertura) *60;
        //printf("%d", simulador.tempo_aberto);
        printf("│    ★ O parque estará aberto durante %d mins.\n", simulador.tempo_aberto);
}

void * virtualtime() // função que incrementa variavel do tempo virtual
{
        while (simulador.aberto) {
                usleep(150000);
                simulador.contador_time++;
        }

        pthread_exit(NULL);
}

char * gettime() // funçao para obter o tempo virtual
{
        int horas = simulador.hora_de_abertura;
        int min;

        horas = horas + simulador.contador_time/60;
        min = simulador.contador_time % 60;

        if(horas >= 24) horas = 0;  //para a hora do dia não ultrapassar 24

        char * horasAtual = (char *) malloc(sizeof(char) * 3);
        sprintf(horasAtual, "%dh%d", horas, min);
        return horasAtual;
}

void *divertimento()
{
        while(simulador.aberto) {
                if (simulador.contador_time >= simulador.tempo_aberto) {
                        simulador.divertimento_boolean = 0;
                }
        }
}

void *fury325(int socket)
{
        while(simulador.aberto) {

                printf("│%s ★ Montanha russa Fury 325 pronta para receber clientes.\n", gettime());
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
                                if(simulador.aberto){
                                sem_wait(&s_cliente_verificado);
                                pessoas_verificadas++;
                              }
                              else pessoas_verificadas = 4;
                        }
                        if(simulador.aberto){
                        printf("│%s ★ Montanha russa Fury 325 começou uma viagem.\n", gettime());
                        sleep(4);
                        printf("│%s ★ Montanha russa Fury 325 terminou uma viagem.\n", gettime());
                        for ( int i = 0; i < pessoas_fury; i++) {
                                sem_post(&s_finish_fury); // assinala que acabou a volta
                        }
                      }
                }
        }
        for ( int i = 0; i < 4; i++) {
                sem_post(&s_finish_fury); // assinala que acabou a volta
        }
        for ( int i = 0; i < num_pessoas_fury325; i++) {
                sem_post(&s_cliente_tempo);
        }

        pthread_exit(NULL);
}

void *cliente_fury(int id)
{
        pthread_mutex_lock(&mutex_comunicacao);
        sprintf(lineCriacao, "%s;%d;4;e", gettime(),id);
        printf("│%s • O cliente %d esta na fila para entrar na montanha russa Fury 325.\n", gettime(), id);
        write(newsockfd, lineCriacao, strlen(lineCriacao));
        while(strcmp(line, "q") != 0)
        {
                read(newsockfd, line, MAXLINE);
        }
        strcpy(line, " ");
        usleep(150000);
        pthread_mutex_unlock(&mutex_comunicacao);

        pthread_mutex_lock(&mutex_fury);
        num_pessoas_fury325++;
        sem_post(&s_fury325);
        // avisa ao fury que tem clientes
        pthread_mutex_unlock(&mutex_fury);
        sem_wait(&s_cliente_tempo);
        if (simulador.contador_time < simulador.tempo_aberto -20 && cliente[id].tempo_max_espera > (simulador.contador_time - cliente[id].tempo_chegou_divertimento)) {
                sem_wait(&s_cliente_fury325); // retira os cliente da fila (estão dentro do fury)

                pthread_mutex_lock(&mutex_comunicacao);
                sprintf(lineCriacao, "%s;%d;2;e", gettime(),id);
                printf("│%s • O cliente %d entrou na montanha russa Fury 325.\n", gettime(), id);
                write(newsockfd, lineCriacao, strlen(lineCriacao));
                while(strcmp(line, "q") != 0)
                {
                        read(newsockfd, line, MAXLINE);
                }
                strcpy(line, " ");
                usleep(150000);
                pthread_mutex_unlock(&mutex_comunicacao);

                sem_post(&s_cliente_verificado);
                sem_wait(&s_finish_fury); // retira os clientes do fury
        }
        else{
                pthread_mutex_lock(&mutex_fury);
                sem_post(&s_cliente_tempo);
                num_pessoas_fury325--;
                pthread_mutex_unlock(&mutex_fury);

                pthread_mutex_lock(&mutex_comunicacao);
                sprintf(lineCriacao, "%s;%d;6;e", gettime(),id);
                printf("│%s • O cliente %d desistiu de entrar na montanha russa Fury 325.\n", gettime(), id);
                write(newsockfd, lineCriacao, strlen(lineCriacao));
                while(strcmp(line, "q") != 0)
                {
                        read(newsockfd, line, MAXLINE);
                }
                strcpy(line, " ");
                usleep(150000);
                pthread_mutex_unlock(&mutex_comunicacao);
        }
}

void * takabisha (int socket)
{
        while (simulador.aberto)
        {
                //int pessoas_para_entrar = 0;
                //printf("TAKABISHA preparado para andar.\n" );
                printf("│%s ★ Montanha russa Takabisha pronta para receber clientes.\n", gettime());
                //  while (pessoas_para_entrar < 2 ) {
                sem_wait(&s_takabisha);
                if(simulador.tempo_aberto > simulador.contador_time )
                {
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
                        if(simulador.aberto){
                        if (verifica_cliente_takabisha != 1) {
                                // tempo da volta da montanha
                                printf("│%s ★ Montanha russa Takabisha saiu para uma viagem.\n", gettime());
                                sleep(3);
                                printf("│%s ★ Montanha russa Takabisha acabou uma viagem.\n", gettime());
                                //for ( int i = 0; i < pessoas_para_entrar; i++) {
                                sem_post(&s_finish_takabisha);
                                sem_wait(&s_comunicacao_takabisha);
                        }
                        else{
                                verifica_cliente_takabisha = 0;
                        }
                      }
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

        pthread_exit(NULL);
}

void *cliente_takabisha(int id)
{
        pthread_mutex_lock(&mutex_comunicacao);
        sprintf(lineCriacao, "%s;%d;4;d", gettime(),id);
        printf("│%s • O cliente %d esta na fila para entrar na montanha russa Takabisha.\n", gettime(), id);
        write(newsockfd, lineCriacao, strlen(lineCriacao));
        while(strcmp(line, "q") != 0)
        {
                read(newsockfd, line, MAXLINE);
        }
        strcpy(line, " ");
        pthread_mutex_unlock(&mutex_comunicacao);

        pthread_mutex_lock(&mutex_takabisha);
        if (cliente[id].prioridade == 1)
        {
                num_prio_takabisha++;
        }
        else
        {
                num_sem_prio_takabisha++;
        }
        sem_post(&s_takabisha);

        if (cliente[id].prioridade == 1) { // cliente que tem prioridade
                pthread_mutex_unlock(&mutex_takabisha);
                sem_wait(&s_prio_takabisha);
                if(simulador.divertimento_boolean && cliente[id].tempo_max_espera > (simulador.contador_time - cliente[id].tempo_chegou_divertimento)) {

                        pthread_mutex_lock(&mutex_comunicacao);
                        sprintf(lineCriacao, "%s;%d;2;d", gettime(),id);
                        printf("│%s • O cliente %d, com prioridade, entrou na montanha russa Takabisha.\n", gettime(), id);
                        write(newsockfd, lineCriacao, strlen(lineCriacao));
                        while(strcmp(line, "q") != 0)
                        {
                                read(newsockfd, line, MAXLINE);
                        }
                        strcpy(line, " ");
                        pthread_mutex_unlock(&mutex_comunicacao);

                        sem_post(&s_sai_takabisha);
                        sem_wait(&s_finish_takabisha);
                }
                else{
                        if(num_prio_takabisha > 0) {
                                sem_post(&s_prio_takabisha);
                        }
                        else if(num_sem_prio_takabisha > 0) {
                                sem_post(&s_sem_prio_takabisha);
                        }
                        else{
                                verifica_cliente_takabisha = 1;
                                sem_post(&s_sai_takabisha);
                        }
                }

        }
        else{ // cliente sem prioridade
                pthread_mutex_unlock(&mutex_takabisha);
                sem_wait(&s_sem_prio_takabisha);
                if(simulador.divertimento_boolean && cliente[id].tempo_max_espera > (simulador.contador_time - cliente[id].tempo_chegou_divertimento)) {

                        pthread_mutex_lock(&mutex_comunicacao);
                        sprintf(lineCriacao, "%s;%d;2;d", gettime(),id);
                        printf("│%s • O cliente %d entrou na montanha russa Takabisha.\n", gettime(), id);
                        write(newsockfd, lineCriacao, strlen(lineCriacao));
                        while(strcmp(line, "q") != 0)
                        {
                                read(newsockfd, line, MAXLINE);
                        }
                        strcpy(line, " ");
                        pthread_mutex_unlock(&mutex_comunicacao);

                        sem_post(&s_sai_takabisha);
                        sem_wait(&s_finish_takabisha);
                }
                else{
                        if(num_prio_takabisha > 0) {
                                sem_post(&s_prio_takabisha);
                        }
                        else if(num_sem_prio_takabisha > 0) {
                                sem_post(&s_sem_prio_takabisha);
                        }
                        else{
                                verifica_cliente_takabisha = 1;
                        }
                }
        }

        pthread_mutex_lock(&mutex_comunicacao);
        sprintf(lineCriacao, "%s;%d;5;d", gettime(),id);
        printf("│%s • O cliente %d já não se encontra na montanha russa Takabisha.\n", gettime(), id);
        write(newsockfd, lineCriacao, strlen(lineCriacao));
        while(strcmp(line, "q") != 0)
        {
                read(newsockfd, line, MAXLINE);
        }
        strcpy(line, " ");
        pthread_mutex_unlock(&mutex_comunicacao);

        sem_post(&s_comunicacao_takabisha);
}

void *escolhedivertimento(int id)
{
        switch (cliente[id].divertimento)
        {
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
        while(simulador.aberto)
        {
                sem_wait(&s_prod_bilheteira);
                pthread_mutex_lock(&mutex_bilheteira);
                if((rand()%100)< simulador.perc_prioridade) // define se o cliente é prioritário ou não
                {
                        simulador.buff_bilheteira[produz_bilheteira] = 1;
                }
                else
                {
                        simulador.buff_bilheteira[produz_bilheteira] = 2;
                }
                //printf("%d\n",simulador.buff_bilheteira[produz_bilheteira] );
                //printf("%d\n", produz_bilheteira);
                produz_bilheteira = ( produz_bilheteira + 1 )% 4;
                pthread_mutex_unlock(&mutex_bilheteira);
                sem_post(&s_cons_bilheteira);
        }

        pthread_exit(NULL);
}

void bilhete(int id)
{
        //printf("COMPROU BILHETE ESTE FDP");
        if(simulador.aberto) {

                pthread_mutex_lock(&mutex_comunicacao);
                sprintf(lineCriacao, "%s;%d;3;c", gettime(),id);
                printf("│%s • O cliente %d esta na fila para comprar na bilhetaria.\n", gettime(), id);
                write(newsockfd, lineCriacao, strlen(lineCriacao));
                while(strcmp(line, "q") != 0)
                {
                        read(newsockfd, line, MAXLINE);
                }
                strcpy(line, " ");
                usleep(150000);
                pthread_mutex_unlock(&mutex_comunicacao);

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

                //printf("O cliente n%d tem o valor de prioridade %d e o divertimento é %d\n",id, cliente[id].prioridade,cliente[id].divertimento);
                //printf("│%s • O cliente %d esta na fila para comprar na bilhetaria.\n", gettime(), id);
                if(cliente[id].prioridade == 1) printf("│%s • O cliente %d comprou bilhete superQuick para andar na montanha ", gettime(), id);
                else printf("│%s • O cliente %d comprou bilhete para andar na montanha ", gettime(), id);

                if(cliente[id].divertimento == 0) printf("Takabisha.\n" );
                else printf("Fury 325.\n");

                consome_bilheteira = (consome_bilheteira + 1)% 4;
                pthread_mutex_unlock(&mutex_bilheteira);
                sem_post(&s_prod_bilheteira);

                pthread_mutex_lock(&mutex_comunicacao);
                sprintf(lineCriacao, "%s;%d;5;c", gettime(),id);
                printf("│%s • O cliente %d já não se encontra na bilhetaria.\n", gettime(), id);
                write(newsockfd, lineCriacao, strlen(lineCriacao));
                while(strcmp(line, "q") != 0)
                {
                        read(newsockfd, line, MAXLINE);
                }
                strcpy(line, " ");
                usleep(150000);
                pthread_mutex_unlock(&mutex_comunicacao);
        }
        else{
                printf("│%s • O Cliente %d foi forçado a sair do parque, porque vai fechar.\n",gettime(),id );
        }
}

void *vai_bilheteira(int id)
{
        cliente[id].bilheteira = rand()%100;
        /*while (cliente[id].bilheteira > simulador.perc_cliente_bilhete) { // ciclo para fazer os cliente perder tempo para irem para a bilhetaria !
                cliente[id].bilheteira = rand()%100;
                printf("O cliente n%d está a espera.\n", id);
           }*/
        sleep(3);
        if (cliente[id].bilheteira < simulador.perc_cliente_bilhete)
        {
                //printf("O cliente n%d ta na bilheteira.\n",id);
                if(simulador.divertimento_boolean)
                {
                        bilhete(id);
                }
                else{
                        printf("│%s ★ A bilheteira está fechada.\n", gettime());
                }
        }
        else{
                cliente[id].divertimento = 2;
                //printf("O divertimento do sacana %d", cliente[id].divertimento);
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

        sem_wait(&s_tam_max_parque);     // fazer post disto, ainda estou a pensar onde

        pthread_mutex_lock(&mutex_comunicacao);
        sprintf(lineCriacao, "%s;%d;2;b", gettime(),id);
        printf("│%s • O cliente %d entrou no recinto do parque.\n", gettime(), id);
        write(newsockfd, lineCriacao, strlen(lineCriacao));
        while(strcmp(line, "q") != 0)
        {
                read(newsockfd, line, MAXLINE);
        }
        strcpy(line, " ");
        usleep(150000);
        pthread_mutex_unlock(&mutex_comunicacao);

        if(simulador.aberto) //não tenta andar nas coisas se já estiver fechado
        {
                vai_bilheteira(id);
                if(cliente[id].divertimento == 2) {
                        printf("│%s • O cliente %d afinal não vai andar nas montanhas russas.\n", gettime(), id );
                }
                else{
                        escolhedivertimento(id);
                }
        }
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

        pthread_mutex_lock(&mutex_comunicacao);
        sprintf(lineCriacao, "%s;%d;5;b", gettime(),id);
        printf("│%s • O Cliente %d saiu do parque.\n", gettime(), id );
        //printf("%s\n", "CLiente saiu");
        write(newsockfd, lineCriacao, strlen(lineCriacao));
        while(strcmp(line, "q") != 0)
        {
                read(newsockfd, line, MAXLINE);
        }
        strcpy(line, " ");
        usleep(150000);
        pthread_mutex_unlock(&mutex_comunicacao);
        sem_post(&s_tam_max_parque);

        pthread_exit(NULL);

}

void *cria_cliente(int socket)
{
        newsockfd = socket;
        //printf("%s\n",  simulador.clientes_criados);
        for (int i = 0; i < simulador.clientes_criados; i++) {

                if(!simulador.aberto) break;
                usleep(150000);
                usleep(150000);
                usleep(150000);

                if((pthread_create(&(t_cliente[i]),NULL,(void *)trata_cliente, i))!=0)
                {
                        err_dump("pthread_create: erro criação thread");
                }
                else{
                        conta_cliente++;
                        pthread_mutex_lock(&mutex_comunicacao);
                        sprintf(lineCriacao, "%s;%d;1;a", gettime(),i);
                        printf("│%s • O cliente %d chegou ao recinto do parque.\n", gettime(), i);
                        write(newsockfd, lineCriacao, strlen(lineCriacao));
                        while(strcmp(line, "q") != 0)
                        {
                                read(newsockfd, line, MAXLINE);
                        }
                        strcpy(line, " ");
                        usleep(150000);
                        pthread_mutex_unlock(&mutex_comunicacao);
                }

        }

        pthread_exit(NULL); //acaba a thread
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
                        printf("│%s ★ As  bilheteiras já abriram.\n", gettime());
                }
                usleep(150000 * (rand()%100));
        }

        pthread_exit(NULL);
}
