#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
main() {
	struct sockaddr_in server;
	int sd1, n = sizeof server;
	sd1 = socket(PF_INET, SOCK_DGRAM, 0);
	server.sin_family = PF_INET;
	server.sin_port = 8000;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(sd1, (struct sockaddr *) &server, sizeof server);
	int port = 9000; char url[100];
	recvfrom(sd1, url, sizeof url, 0, (struct sockaddr *) &server, &n);
	printf("\n URL : %s", url);
	sendto(sd1, &port, sizeof port, 0, (struct sockaddr *) &server, sizeof server);	
}
