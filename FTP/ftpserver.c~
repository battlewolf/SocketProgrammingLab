#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

struct sockaddr_in ss1, ss2;
int sd1, sd2;

struct sockaddr_in cs1;
int csd1;

void clientConn() {
	csd1 = socket(PF_INET, SOCK_STREAM, 0);
	cs1.sin_addr.s_addr = inet_addr("127.0.0.1");
	cs1.sin_family = PF_INET;
	cs1.sin_port = 9875;
	connect(csd1, (struct sockaddr *) &cs1, sizeof cs1);
}

void serverConn() {
	sd1 = socket(PF_INET, SOCK_STREAM, 0);
	ss1.sin_addr.s_addr = inet_addr("127.0.0.1");
	ss1.sin_family = PF_INET;
	ss1.sin_port = 9876;

	bind(sd1, (struct sockaddr *) &ss1, sizeof ss1);
	listen(sd1, 3);
	int n = sizeof ss2;
	sd2 = accept(sd1, (struct sockaddr *) &ss2, &n);	
}

int checkUser() {
	char username[100], password[100];
	
	recv(sd2, username, sizeof username, 0);
	recv(sd2, password, sizeof password, 0);
	printf("\n Username : %s password : %s", username, password);
/*	int val1 = strcmp(username, "ab"), val2 = strcmp(password, "cd"), flag;*/
/*	printf("\n val1 : %d val2 : %d", val1, val2);*/
/*	if(flag = (val1 == 0 && val2 == 0)) {*/
/*			printf("\n Valid User ");*/
/*			send(sd2, &flag, sizeof flag, 0); close(sd1); close(sd2);*/
/*			return 1;*/
/*	}*/
/*	else {*/
/*		printf("\n Invalid User ");*/
/*		send(sd2, &flag, sizeof flag, 0);*/
/*		close(sd1); close(sd2);*/
/*		return -1;*/
/*	}*/
return 1;
}
main() {
	serverConn();
	if(checkUser() == 1) {
		printf("\n I am in");		
	}
}
