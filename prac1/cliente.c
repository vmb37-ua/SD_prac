#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int
main (int argc, char *argv[])
{
	char *servidor_ip;
	char *servidor_puerto;
	char *scolor;
	char color;
	char *simporte;
	long importe;
	char respuesta[1024];
	struct sockaddr_in direccion;
	int s;
	int n, enviados, recibidos;

	/* Comprobar los argumentos */
	if (argc !=  5)
	{
		fprintf(stderr, "Error. Debe indicar la direccion del servidor (IP y Puerto), el color elegido y el simporte a apostar\r\n");
		fprintf(stderr, "Sintaxis: %s <ip> <puerto> <color>[R, N, V] <simporte>\n\r", argv[0]);
		fprintf(stderr, "Ejemplo : %s 192.168.6.7 8574 \"Esto es un mensaje\"\n\r", argv[0]);
		return 1;
	}

	/* Tomar los argumentos */		
	servidor_ip = argv[1];
	servidor_puerto = argv[2];
	scolor = argv[3];
	simporte = argv[4];

	importe = strtol(simporte, NULL, 10);
	color = scolor[0];

	if(color != 'R' && color != 'V' && color != 'N'){
		fprintf(stderr, "Error: Color no válido (R, V, N)");
		return 1;
	}

	if(importe < 0 || importe > 1000000) {
		fprintf(stderr, "Error: importe debe ser en el rango válido (0-1000000)");
		return 1;
	}

	printf("\n\rEnviar apuesta \"%c %s\" a %s:%s...\n\r\n\r", color, simporte, servidor_ip, servidor_puerto);

	/**** Paso 1: Abrir el socket ****/

	s = socket(AF_INET, SOCK_STREAM, 0); /* creo el socket */
	if (s == -1)
	{
		fprintf(stderr, "Error. No se puede abrir el socket\n\r");
		return 1;
	}
	printf("Socket abierto\n\r");

	/**** Paso 2: Conectar al servidor ****/		

	/* Cargar la direcci�n */
	direccion.sin_family = AF_INET; /* socket familia INET */
	direccion.sin_addr.s_addr = inet_addr(servidor_ip);
	direccion.sin_port = htons(atoi(servidor_puerto));
	
	if (connect(s, (struct sockaddr *)&direccion, 	sizeof (direccion)) == -1)
	{
		fprintf(stderr, "Error. No se puede conectar al servidor\n\r");
		close(s);
		return 1;
	}
	printf("Conexi�n establecida\n\r");

	/**** Paso 3: Enviar mensaje ****/
	
	int seguir_jugando = 1;

	n = 1; // bytes de char 
	enviados = write(s, scolor, n);
	if (enviados == -1 || enviados < n)
	{
		fprintf(stderr, "Error enviando el color\n\r");
		close(s);
		return 1;
	}

	printf("Color enviado\n\r");

	n = sizeof(long); 
	enviados = write(s, &(importe), n);
	if (enviados == -1 || enviados < n)
	{
		fprintf(stderr, "Error enviando el importe\n\r");
		close(s);
		return 1;
	}

	printf("importe enviado\n\r");

	/**** Paso 4: Recibir respuesta ****/

	n = sizeof(respuesta) - 1;
	recibidos = read(s, respuesta, n);
	if (recibidos == -1)
	{
		fprintf(stderr, "Error recibiendo respuesta\n\r");
		close(s);
		return 1;
	}
	respuesta[recibidos] = '\0';
	printf("Has ganado %s monedas porque ha salido el color ", respuesta);
	switch (color){
		case 86: printf("verde\n");break;
		case 78: printf("negro\n");break;
		case 82: printf("rojo\n");break;
	}

	/**** Paso 5: Cerrar el socket ****/
	close(s);
	printf("Socket cerrado. Comunicaci�n finalizada\n\r");

	return 0;
}

