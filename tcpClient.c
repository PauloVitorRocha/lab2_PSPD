/* ******************************/
/* FGA/Eng. Software/ FRC       */
/* Prof. Fernando W. Cruz       */
/* Codigo: tcpClient2.c         */
/* ******************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 80
#define VECTOR_SIZE 10000

struct vetorDeFloats
{
	float x[VECTOR_SIZE];
} vetorDeFloats;

struct maxMinRes
{
	float max;
	float min;
} maxMinRes;

struct ipPorta
{
	char ip[100];
	char porta[20];
} ipPorta;

struct vetorDeFloats vetorzinho;
struct maxMinRes maxMin;

void *func(void *arg)
{
	struct ipPorta *ipPorta = (struct ipPorta *)arg;

	struct sockaddr_in ladoServ1; /* contem dados do servidor 	*/
	int sd;						 /* socket descriptor              */
	int n, k;					 /* num caracteres lidos do servidor */

	memset((char *)&ladoServ1, 0, sizeof(ladoServ1)); /* limpa estrutura */

	ladoServ1.sin_family = AF_INET;					   /* config. socket p. internet*/
	ladoServ1.sin_addr.s_addr = inet_addr(ipPorta->ip); /*ip*/
	ladoServ1.sin_port = htons(atoi(ipPorta->porta));   /*porta*/

	/* Cria socket */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		fprintf(stderr, "Criacao do socket falhou!\n");
		exit(1);
	}

	/* Conecta socket ao servidor definido */
	if (connect(sd, (struct sockaddr *)&ladoServ1, sizeof(ladoServ1)) < 0)
	{
		fprintf(stderr, "Tentativa de conexao falhou no athread!\n");
		exit(1);
	}

	printf("Inside thread\n");
	send(sd, &vetorzinho, sizeof(vetorzinho), 0); /* enviando dados ...  */
	recv(sd, &maxMin, sizeof(maxMin), 0);
	printf("Resposta do Sv: Max = %f and Min = %f\n", maxMin.max, maxMin.min);
	printf("------- encerrando conexao com o servidor -----\n");

	close(sd);

	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		vetorzinho.x[i] = pow(i - VECTOR_SIZE / 2, 2);
	}
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		vetorzinho.x[i] = sqrt(vetorzinho.x[i]);
	}
	for (int i = 0; i < VECTOR_SIZE; i++)
	{
		printf("v[%d] = %f\n", i, vetorzinho.x[i]);
	}

	struct sockaddr_in ladoServ; /* contem dados do servidor 	*/
	int sd;						 /* socket descriptor              */
	int n, k;					 /* num caracteres lidos do servidor */

	/* confere o numero de argumentos passados para o programa */
	if (argc < 5)
	{
		printf("uso correto: %s <ip_do_servidor> <porta_do_servidor> <ip_do_outro_servidor> <porta_do_outro_servidor>\n", argv[0]);
		exit(1);
	}
	struct ipPorta *ipPorta;
	ipPorta = malloc(sizeof(struct ipPorta));
	strcpy((*ipPorta).ip, argv[1]);
	strcpy((*ipPorta).porta, argv[2]);

	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, func, (void *)ipPorta);

	memset((char *)&ladoServ, 0, sizeof(ladoServ)); /* limpa estrutura */

	ladoServ.sin_family = AF_INET;				   /* config. socket p. internet*/
	ladoServ.sin_addr.s_addr = inet_addr(argv[3]); /*ip*/
	ladoServ.sin_port = htons(atoi(argv[4]));	   /*porta*/

	/* Cria socket */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		fprintf(stderr, "Criacao do socket falhou!\n");
		exit(1);
	}

	/* Conecta socket ao servidor definido */
	if (connect(sd, (struct sockaddr *)&ladoServ, sizeof(ladoServ)) < 0)
	{
		fprintf(stderr, "Tentativa de conexao falhou na main!\n");
		exit(1);
	}

	printf("\n\n\n\n\nInside Main \n");
	send(sd, &vetorzinho, sizeof(vetorzinho), 0); /* enviando dados ...  */
	recv(sd, &maxMin, sizeof(maxMin), 0);
	printf("Resposta do Sv: Max = %f and Min = %f\n", maxMin.max, maxMin.min);

	printf("------- encerrando conexao com o servidor -----\n");
	close(sd);
	pthread_join(thread1, NULL);
	return (0);
} /* fim do programa */
