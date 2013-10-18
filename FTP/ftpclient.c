#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>

struct sockaddr_in ss1, ss2;
int sd1, sd2;

struct sockaddr_in cs1;
int csd1;

void clientConn() {
	csd1 = socket(PF_INET, SOCK_STREAM, 0);
	cs1.sin_addr.s_addr = inet_addr("127.0.0.1");
	cs1.sin_family = PF_INET;
	cs1.sin_port = 9876;
	connect(csd1, (struct sockaddr *) &cs1, sizeof cs1);
}

void serverConn() {
	sd1 = socket(PF_INET, SOCK_STREAM, 0);
	ss1.sin_addr.s_addr = inet_addr("127.0.0.1");
	ss1.sin_family = PF_INET;
	ss1.sin_port = 9875;

	bind(sd1, (struct sockaddr *) &ss1, sizeof ss1);
	listen(sd1, 3);
	int n = sizeof ss2;
	sd2 = accept(sd1, (struct sockaddr *) &ss2, &n);	
}

main() {
	{
		char username[100], password[100];
		clientConn();
		printf("\n Username and Password :");
		scanf("%s %s", username, password);
		send(csd1, username, sizeof username, 0);
		send(csd1, password, sizeof password, 0);
//		int flag;
//		recv(csd1, &flag, sizeof flag, 0);
//		if(flag == 1) printf("\n yes!");
//		else printf("\n Get lost!");
//		close(csd1);
	}
}
