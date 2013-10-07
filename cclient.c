#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
main()
{
	char filename[100], buff[1000];
	struct sockaddr_in cl; int csd;
	csd = socket(PF_INET, SOCK_STREAM, 0);
	cl.sin_addr.s_addr = inet_addr("127.0.0.1");
	cl.sin_family = PF_INET;
	cl.sin_port = 8000;
	connect(csd, (struct sockaddr *) &cl, sizeof cl);
	strcpy(filename, "iclient.c");
	send(csd, filename, sizeof filename, 0);
	recv(csd, buff, sizeof buff, 0);
	printf("\n");
	printf("File contents are : %s", buff);
	close(csd);
}
