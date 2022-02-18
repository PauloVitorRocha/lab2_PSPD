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

#define MAX_SIZE 80
#define VECTOR_SIZE 10001

struct vetorDeFloats
{
	float x[VECTOR_SIZE];
} vetorDeFloats;

struct vetorDeFloats vetorzinho;

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

	printf("size - %lu\n", sizeof(vetorzinho));
	struct sockaddr_in ladoServ; /* contem dados do servidor 	*/
	int sd;						 /* socket descriptor              */
	int n, k;					 /* num caracteres lidos do servidor */

	/* confere o numero de argumentos passados para o programa */
	if (argc < 3)
	{
		printf("uso correto: %s <ip_do_servidor> <porta_do_servidor>\n", argv[0]);
		exit(1);
	}

	memset((char *)&ladoServ, 0, sizeof(ladoServ)); /* limpa estrutura */

	ladoServ.sin_family = AF_INET;				   /* config. socket p. internet*/
	ladoServ.sin_addr.s_addr = inet_addr(argv[1]); /*ip*/
	ladoServ.sin_port = htons(atoi(argv[2]));	   /*porta*/

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
		fprintf(stderr, "Tentativa de conexao falhou!\n");
		exit(1);
	}

	printf("> ");
	send(sd, &vetorzinho, sizeof(vetorzinho), 0); /* enviando dados ...  */

	printf("------- encerrando conexao com o servidor -----\n");
	close(sd);
	return (0);
} /* fim do programa */
