#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include "../TCP.h"

#define FRAME_SIZE 100
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
	printf("Profession : %s\n", user->profession); 
}

int main(int argc, char* argv[]){
		
 
 	int server_sockfd;
	struct sockaddr_in address;
	const int PORT = atoi(argv[1]);

	TCP_server_init(&server_sockfd, &address, PORT);

	int new_socket = TCP_server_accept(&server_sockfd, &address, PORT);

	FILE* fptr = fopen("../small-orderly-data.txt", "r"); 

	if(!fptr) error_and_exit("Couldn't read from file");

	User user = {{0}, 0, {0}};
	char response[RESPONSE_SIZE] = {0};

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT_SECONDS; // Setting a timeout of 5 seconds
	timeout.tv_usec = 0;

	// Associate the timeout with recv()
	if(setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&timeout), sizeof(timeout)) < 0) error_and_exit("Couldn't assert timeout");

	int frame_ind = 0;
	while(fscanf(fptr, "%s %d %s", user.name, &user.age, user.profession) != EOF){
		send(new_socket, &user, sizeof(User), 0);
		int recv_bytes = recv(new_socket, response, RESPONSE_SIZE, 0);
		

		//if(recv_bytes != -1 && atoi(response + 3) != frame_ind){
		if(recv_bytes == -1){
			printf("Frame %d lost !! Retransmitting\n", frame_ind);
			fseek(fptr, -sizeof(user), SEEK_CUR);
		}else{
			printf("%s\n", response);
			frame_ind++;
		}

		bzero(response, RESPONSE_SIZE);

	}


	TCP_close(server_sockfd);
	TCP_close(new_socket);

	return 0;
}
