#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
int main(int argc, char *argv[])
{
	struct sockaddr_in server;
	char smessage[50], rmessage[50];
	int s, c;
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	server.sin_family = AF_INET;
	server.sin_port = 8000;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(s, (struct sockaddr *) &server, sizeof(server)) == -1)
	{
		perror("connect failed");
		return -1;
	}
	
	while(1)
	{
		fscanf(stdin,"%s",smessage);
		send(s, smessage, sizeof(smessage), 0);
		recv(s, rmessage, sizeof(rmessage), 0);
		if(strcmp(rmessage,"bye") == 0) return -1;
		fprintf(stdout, "%s", rmessage);				
	}
	return 0;
} 
