#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
main() 	
{
	char mess[100];
	int sd1, sd2;
	struct sockaddr_in ss1, ss2;
	sd1 = socket(PF_INET, SOCK_STREAM, 0);
	ss1.sin_addr.s_addr = inet_addr("127.0.0.1");
	ss1.sin_port = 9000;
	ss1.sin_family = PF_INET;
	bind(sd1, (struct sockaddr *) &ss1, sizeof ss1);
	listen(sd1, 3);
	int n = sizeof ss2;
	while(1)
	{
		sd2 = accept(sd1, (struct sockaddr *) &ss2, &n);
		recv(sd2, mess, sizeof mess, 0);
		printf("\n Client says : %s", mess);
		scanf("%s", mess);
		send(sd2, mess, sizeof mess, 0);
	}
	close(sd1); close(sd2);
}
