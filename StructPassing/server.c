#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8000

struct message {
	int age;
	char name[100];	
};

//socket variables
struct sockaddr_in s11, s12;
int sd1, sd2;

void makeConnection() {
	sd1 = socket(AF_INET, SOCK_STREAM, 0);
	s11.sin_addr.s_addr = inet_addr("127.0.0.1");
	s11.sin_family = PF_INET;
	s11.sin_port = PORT;
	
	bind(sd1, (struct sockaddr *) &s11, sizeof(s11));
	listen(sd1, 2);
	int n = sizeof(s12);
	
	sd2 = accept(sd1, (struct sockaddr *) &s12, &n);
}

void killConn() {
	close(sd1);
	close(sd2);
}

main() {
	makeConnection();
	struct message M;
	recv(sd2, &M, sizeof(M), 0);
	printf("\n Name : %s Age : %d", M.name, M.age);
	killConn();
}
