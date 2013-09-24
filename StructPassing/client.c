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

struct sockaddr_in s;
int csd;

void makeClientConnection() {
	csd = socket(AF_INET, SOCK_STREAM, 0);
	s.sin_addr.s_addr = inet_addr("127.0.0.1");
	s.sin_family = PF_INET;
	s.sin_port = PORT;
	connect(csd, (struct sockaddr *) &s, sizeof(s));
}

main() {
	struct message M;
	printf("\n Age :");
	scanf("%d", &(M.age));
	printf("\n Name :");
	scanf("%s", M.name);
	makeClientConnection();
	send(csd, &M, sizeof(M), 0);
	close(csd);
}
