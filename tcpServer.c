/* *****************************/
/* FGA / Eng. Software / FRC   */
/* Prof. Fernando W. Cruz      */
/* Codigo: tcpServer2.c	       */
/* *****************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define QLEN 5      /* tamanho da fila de clientes  */
#define MAX_SIZE 80 /* tamanho do buffer */

#define VECTOR_SIZE 10001

struct vetorDeFloats
{
  float x[VECTOR_SIZE];
} vetorDeFloats;

struct maxMinRes
{
  float max;
  float min;
} maxMinRes;

int atende_cliente();
int main(int argc, char *argv[])
{
  struct sockaddr_in endServ; /* endereco do servidor   */
  struct sockaddr_in endCli;  /* endereco do cliente    */
  int sd, novo_sd;            /* socket descriptors */
  int pid, alen, n;

  if (argc < 3)
  {
    printf("Digite IP e Porta para este servidor\n");
    exit(1);
  }
  memset((char *)&endServ, 0, sizeof(endServ)); /* limpa variavel endServ    */
  endServ.sin_family = AF_INET;                 /* familia TCP/IP   */
  endServ.sin_addr.s_addr = inet_addr(argv[1]); /* endereco IP      */
  endServ.sin_port = htons(atoi(argv[2]));      /* PORTA	    */

  /* Cria socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
  {
    fprintf(stderr, "Falha ao criar socket!\n");
    exit(1);
  }

  /* liga socket a porta e ip */
  if (bind(sd, (struct sockaddr *)&endServ, sizeof(endServ)) < 0)
  {
    fprintf(stderr, "Ligacao Falhou!\n");
    exit(1);
  }

  /* Ouve porta */
  if (listen(sd, QLEN) < 0)
  {
    fprintf(stderr, "Falhou ouvindo porta!\n");
    exit(1);
  }

  printf("Servidor ouvindo no IP %s, na porta %s ...\n\n", argv[1], argv[2]);
  /* Aceita conexoes */
  alen = sizeof(endCli);
  for (;;)
  {
    /* espera nova conexao de um processo cliente ... */
    if ((novo_sd = accept(sd, (struct sockaddr *)&endCli, &alen)) < 0)
    {
      fprintf(stdout, "Falha na conexao\n");
      exit(1);
    }
    fprintf(stdout, "Cliente %s: %u conectado.\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port));
    atende_cliente(novo_sd, endCli);
  } /* fim for */
} /* fim do programa */

int atende_cliente(int descritor, struct sockaddr_in endCli)
{
  struct vetorDeFloats vetorzinho;
  struct maxMinRes maxMin;
  int n,z;
  maxMin.max = 0;
  maxMin.min = 1e10;

  memset(&vetorzinho, 0x0, sizeof(vetorzinho));
  n = recv(descritor, &vetorzinho, sizeof(vetorzinho), 0);
  for (int i = 0; i < 10000; i++)
  {
    if(vetorzinho.x[i]>maxMin.max){
      maxMin.max = vetorzinho.x[i];
    }
    if(vetorzinho.x[i]<maxMin.min){
      maxMin.min = vetorzinho.x[i];
    }
    
  }
  z = send(descritor, &maxMin, sizeof(maxMin), 0);
  fprintf(stdout, "[%s:%u] => %f\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port), vetorzinho.x[0]);

  fprintf(stdout, "Encerrando conexao com %s:%u ...\n\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port));
  close(descritor);
} /* fim atende_cliente */
