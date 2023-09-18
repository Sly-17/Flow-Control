#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>



void error_and_exit(const char* error){
	fprintf(stderr, "ERROR : %s\n", error);
	exit(EXIT_FAILURE);
}

void TCP_client_connect(int* client_sockfd, struct sockaddr_in* server_address, const int PORT, const char* IP){
	if((*client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) error_and_exit("Could not create socket !!");

	server_address->sin_port = PORT;
	server_address->sin_family = AF_INET;
	server_address->sin_addr.s_addr = inet_addr(IP);

	if(connect(*client_sockfd, (struct sockaddr*)(server_address), sizeof(struct sockaddr_in)) < 0) error_and_exit("Couldn't connect socket to PORT, PORT taken !!");

}




void TCP_server_init(int* server_sockfd, struct sockaddr_in* address, const int PORT){
	
	if((*server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) error_and_exit("Couldn't create socket !!");

	address->sin_port = PORT;
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	int address_len = sizeof(struct sockaddr_in);

	// Bind
	if(bind(*server_sockfd, (struct sockaddr*)(address), address_len) < 0) error_and_exit("Bind Error !!");

	// Listen
	if(listen(*server_sockfd, 4) < 0) error_and_exit("Listen Error !!");

}


int TCP_server_accept(int* server_sockfd, struct sockaddr_in* address, const int PORT){
	
	int address_len = sizeof(struct sockaddr_in);
	int new_socket;

	if((new_socket = accept(*server_sockfd, (struct sockaddr*)(address), (socklen_t*)(&address_len))) < 0) error_and_exit("Accept Error !!");

	return new_socket;

}

void TCP_close(const int sockfd){
	close(sockfd);
}
