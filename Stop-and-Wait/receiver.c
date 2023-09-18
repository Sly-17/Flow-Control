#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include<sys/types.h>
#include <string.h>
#include <time.h>
#include "../TCP.h"


#define RESPONSE_SIZE 15
#define TIMEOUT_SECONDS 3
#define MAX_FRAMES 8

typedef struct{
	char name[100];
	unsigned int age;
	char profession[200];
} User;
	


void print_user(User* user){
	printf("Name       : %s\n", user->name);
	printf("Age        : %d\n", user->age);
	printf("Profession : %s\n\n", user->profession); 
}

int main(int argc, char* argv[]){

	int client_sockfd;
	const int PORT = atoi(argv[1]);
	struct sockaddr_in server_address;


	TCP_client_connect(&client_sockfd, &server_address, PORT, "127.0.0.1");

	User user = {{0}, 0, {0}};

	char ack[RESPONSE_SIZE] = {0};

	int ack_ind = 0;
	
	while(ack_ind < MAX_FRAMES){
		recv(client_sockfd, &user, sizeof(User), 0);
		int timeout = rand() % (TIMEOUT_SECONDS  * 2);
		printf("\nTimeout %d\n", timeout);
		sleep(timeout);
		if(timeout > TIMEOUT_SECONDS) continue;

		sprintf(ack, "ACK%d", ack_ind);
	
		printf("FRAME %d:\n", ack_ind++);
		print_user(&user);
		send(client_sockfd, ack, RESPONSE_SIZE, 0);

	}




	TCP_close(client_sockfd);

	return 0;
}
