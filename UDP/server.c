#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(void)
{
				struct sockaddr_in s_addr, c_addr;
				s_addr.sin_family = AF_INET;
				s_addr.sin_port = 3048;
				s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

				int server = socket(AF_INET, SOCK_DGRAM, 0);
				int b = bind(server, (struct sockaddr*) &s_addr, sizeof s_addr);
				char buff[20];
				int n = sizeof c_addr;
				while(1) {

								recvfrom(server, buff, sizeof buff, 0, (struct sockaddr *) &c_addr, &n);
								if(strcmp(buff, "exit") == 0) {
												strcpy(buff, "bye");
												sendto(server, buff, sizeof buff, 0, (struct sockaddr *) &c_addr, n);
												break;
								}	
								printf("\n Client says : %s ", buff);
								printf("\n Server : ");
								scanf("%s", buff);
								sendto(server, buff, sizeof buff, 0, (struct sockaddr *) &c_addr, n);

				}

				close(server);
				return 0;
}
