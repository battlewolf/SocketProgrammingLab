#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
main()
{
	struct sockaddr_in cs1;
	int csd;
	csd = socket(PF_INET, SOCK_STREAM, 0);
	cs1.sin_addr.s_addr = inet_addr("127.0.0.1");
	cs1.sin_family = PF_INET;
	cs1.sin_port = 9000;
	connect(csd, (struct sockaddr *) &cs1, sizeof cs1);
	while(1)
	{
		char mess[100];
		scanf("%s", mess);
		send(csd, mess, sizeof mess, 0);
		recv(csd, mess, sizeof mess, 0);
		printf("\n Server Says : %s", mess);
	}
	close(csd);
}
