#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  	int fd;
	printf("[server] creating pipe\n");
	mkfifo(PIPE_SERVER,0666);
	printf("[server] reading from pipe\n");	//blocking?
	fd = open(PIPE_SERVER, O_RDONLY); //quit?
	
	char *buffer;
	//while(1){
	buffer = malloc(BUFFER_SIZE);
	read(fd,buffer,HANDSHAKE_BUFFER_SIZE);
	//printf("%s\n",buffer);
	printf("[server] received client pipe: %s\n",buffer);
	close(fd);
	printf("[server] connecting to the client pipe\n");
	fd = open(buffer,O_WRONLY);
	printf("[server] %s\n",strerror(errno));
	printf("[server] %s\n",buffer);
	printf("[server] sending response\n");
	char * char_buf;
	char_buf = malloc(BUFFER_SIZE);
	char_buf = ACK;
	write(fd,char_buf,HANDSHAKE_BUFFER_SIZE);
	//free(buffer);
	//}
	remove(PIPE_SERVER);
	return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  	int fd;
	
	//printf("[update] enter server pipe fd\n");
	char *buffer;
	buffer = malloc(BUFFER_SIZE);
	//scanf("%s",buffer);
	printf("[client] establishing connection\n");
	fd = open(PIPE_SERVER,O_WRONLY);
	if(fd == -1){
		printf("[client] wkp is not there, force quit\n");
		exit(1);
	} 
	printf("[client] creating pipe\n");
	sprintf(buffer, "%d",getpid());
	mkfifo(buffer,0666);
	write(fd,buffer,HANDSHAKE_BUFFER_SIZE);
	close(fd);	
	
	printf("[client] wait for server response\n");
	fd = open(buffer,O_RDONLY);
	char * char_buf;
	char_buf = malloc(BUFFER_SIZE);	
	read(fd,char_buf,HANDSHAKE_BUFFER_SIZE);
	printf("[client] server's response: %s\n",char_buf);	//blocking?
	close(fd);
	remove(buffer);
	return 0;
}
