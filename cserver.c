#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
main() {
	struct sockaddr_in ss1, ss2;
	int sd1, sd2;
	sd1 = socket(PF_INET, SOCK_STREAM, 0);
	ss1.sin_addr.s_addr = inet_addr("127.0.0.1");
	ss1.sin_port = 8000;
	ss1.sin_family = PF_INET;
	bind(sd1, (struct sockaddr *) &ss1, sizeof ss1);
	listen(sd1, 3);
	while(1) {
		int n = sizeof ss2;
		sd2 = accept(sd1, (struct sockaddr *) &ss2, &n);
		size_t pid = fork();
		char filename[100], buff[1000];
		memset(buff, '\0', sizeof buff);
		recv(sd2, filename, sizeof filename, 0);
		printf("\n filename recieved : %s", filename);
		FILE *fptr = fopen(filename, "r");
		int i = 0; char c;
		while(fscanf(fptr, "%c", &c) != EOF) buff[i++] = c;
		buff[i] = '\0';
		fclose(fptr);
		send(sd2, buff, sizeof buff, 0);	
	}
	close(sd1);
	close(sd2);
}
