#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

main()
{
	struct sockaddr_in client1, client2;
	int csd1, csd2, n1 = sizeof client1, n2 = sizeof client2;
	csd1 = socket(PF_INET, SOCK_DGRAM, 0);
	client1.sin_addr.s_addr = inet_addr("127.0.0.1");
	client1.sin_port = 8000;
	client1.sin_family = PF_INET;
	connect(csd1, (struct sockaddr *) &client1, sizeof client1);
	char url[100]; scanf("%s", url);
	sendto(csd1, url, sizeof url, 0, (struct sockaddr *) &client1, n1);
	int my_port;
	recvfrom(csd1, &my_port, sizeof my_port, 0, (struct sockaddr *) &client1, &n1);
	printf("\n Recieved port number : %d", my_port);
	csd2 = socket(PF_INET, SOCK_DGRAM, 0);
	client2.sin_addr.s_addr = inet_addr("127.0.0.1");
	client2.sin_port = my_port;
	client2.sin_family = PF_INET;
	connect(csd2, (struct sockaddr *) &client2, sizeof client2);
	sendto(csd2, url, sizeof url, 0, (struct sockaddr *) &client2, n2);
	char ip[100];
	recvfrom(csd2, ip, sizeof ip, 0, (struct sockaddr *) &client2, &n2);
	printf("\n IP : %s", ip);
}
