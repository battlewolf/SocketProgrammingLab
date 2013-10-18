#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

main() {
	struct sockaddr_in server1, client1;
	int ssd1;
	ssd1 = socket(PF_INET, SOCK_DGRAM, 0);
	server1.sin_family = PF_INET;
	server1.sin_port = 8000;
	server1.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	bind(ssd1, (struct sockaddr *) &server1, sizeof server1);
	char url[100], ip[100];
	int n1 = sizeof client1;
	recvfrom(ssd1, url, sizeof url, 0, (struct sockaddr *) &client1, &n1);
	printf("\n URL : %s", url);
	struct sockaddr_in client2;
	int csd1;
	csd1 = socket(PF_INET, SOCK_DGRAM, 0);
	client2.sin_family = PF_INET;
	client2.sin_port = 9000;
	client2.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(csd1, (struct sockaddr *) &client2, sizeof client2);
	sendto(csd1, url, sizeof url, 0, (struct sockaddr *) &client2, sizeof client2);
	int n2 = sizeof client2;
	recvfrom(csd1, ip, sizeof ip, 0, (struct sockaddr *) &client2, &n2);
	printf("\n IP : %s", ip);
	sendto(ssd1, ip, sizeof ip, 0, (struct sockaddr *) &client1, sizeof client1);
}
