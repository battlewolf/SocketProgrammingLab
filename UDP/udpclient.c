#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

int main(void)
{
	struct sockaddr_in s_addr;
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = 3048;
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int server = socket(AF_INET, SOCK_DGRAM, 0);

	int c = connect(server, (struct sockaddr*) &s_addr, sizeof s_addr);

	char buff[20];
	printf("Enter a word: ");
	scanf("%s", buff);
	sendto(server, buff, sizeof buff, 0, (struct sockaddr *)&s_addr,sizeof s_addr);
	
	int n = sizeof s_addr;
	recvfrom(server, buff, sizeof buff, 0, (struct sockaddr *) &s_addr, &n );
	printf("\n Server : %s", buff);

	close(server);
	return 0;
}
