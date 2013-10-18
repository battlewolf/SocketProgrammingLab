#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
struct node {
	char name[100];
	char ip[100];	
} var[3];

main() {
	struct sockaddr_in server1, client1;
	int ssd1, i; int n = sizeof client1;
	strcpy(var[0].name, "www.google.com");
	strcpy(var[0].ip, "12.2.2.2");
	strcpy(var[1].name, "www.facebook.com");
	strcpy(var[1].ip, "12.2.2.3");
	strcpy(var[2].name, "www.youtube.com");
	strcpy(var[2].ip, "12.5.4.2");
	ssd1 = socket(PF_INET, SOCK_DGRAM, 0); server1.sin_addr.s_addr = inet_addr("127.0.0.1"); server1.sin_family = PF_INET; server1.sin_port = 9000;
	char url[100];
	bind(ssd1, (struct sockaddr *) &server1, sizeof server1);
	recvfrom(ssd1, url, sizeof url, 0, (struct sockaddr *) &client1, &n);
	printf("\n URL : %s", url);
	char ip[100] = "0.0.0.0";
	for(i = 0; i < 3; i++) {
		if(strcmp(var[i].name, url) == 0) {
			strcpy(ip, var[i].ip);
			break;
		}
	}
	sendto(ssd1, ip, sizeof ip, 0, (struct sockaddr *) &client1, n);	
}
