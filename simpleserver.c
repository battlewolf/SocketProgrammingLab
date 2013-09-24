#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
	char smessage[100], rmessage[100];
	int server_s, client_s, n;
	struct sockaddr_in s,c;
	struct in_addr add;
	
	if(inet_aton("127.0.0.1", &add) == -1)
	{
		perror("invalid host");	
		return -1;
	}

	s.sin_port = 8000;
	s.sin_addr = add;
	s.sin_family = AF_INET;

	server_s = socket(AF_INET, SOCK_STREAM, 0);

	if(server_s == -1)
	{
		perror("Trouble in creating socket");
		return -1;
	}
	
	if(bind(server_s, (struct sockaddr *) &s, sizeof(s)) == -1)
	{
		perror("Error in binding the socket to the address");
		return -1;
	}
	
	if(listen(server_s, 3) == -1)
	{
		perror("Error while listening on server side");
		return -1;
	}	
	
	n = sizeof(c);
	client_s = accept(server_s, (struct sockaddr *) &c, &n);
	
	if(client_s == -1)
	{
		perror("error");
		return -1;
	}
	printf("\n Type 0 to exit");
	while(1)
	{
		recv(client_s, rmessage, sizeof(rmessage), 0);
		fprintf(stdout,"%s",rmessage);
		if(strcmp(rmessage,"bye") == 0) return -1;
		
		memset(smessage, '\0', sizeof(smessage));
		fscanf(stdin, "%s", smessage);
		send(client_s, smessage, sizeof(smessage), 0);
	} 
	close(client_s);
	close(server_s);	
	return 0;	
}
