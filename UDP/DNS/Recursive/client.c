#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

main() {
	struct sockaddr_in client;
	int csd;
	csd = socket(PF_INET, SOCK_DGRAM, 0);
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	client.sin_port = 8000;
	client.sin_family = PF_INET;
	connect(csd, (struct sockaddr *) &client, sizeof client);
	char url[100], ip[100];
	scanf("%s", url); int n = sizeof client;
	sendto(csd, url, sizeof url, 0, (struct sockaddr *) &client, sizeof client);
	recvfrom(csd, ip, sizeof ip, 0, (struct sockaddr *) &client, &n);
	printf("\n IP : %s", ip);
}
