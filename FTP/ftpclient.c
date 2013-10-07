#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT1 8000
#define PORT2 9000

int csd1, ssd1, ssd2;
struct sockaddr_in c11, s11, s12;

void clientConn() {
	csd1 = socket(AF_INET, SOCK_STREAM, 0);
	
	c11.sin_addr.s_addr = inet_addr("127.0.0.1");
	c11.sin_port = PORT1;
	c11.sin_family = AF_INET;
	
	connect(csd1, (struct sockaddr *) &c11, sizeof(c11));
}

void serverConn() {
	ssd1 = socket(AF_INET, SOCK_STREAM, 0);
	
	s11.sin_addr.s_addr = inet_addr("127.0.0.1");
	s11.sin_port = PORT2;
	s11.sin_family = AF_INET;
	
	bind(ssd1, (struct sockaddr *) &s11, sizeof(s11));
	listen(ssd1, 3);
	
	int n = sizeof(s12);
	ssd2 = accept(ssd1, (struct sockaddr *) &s12, &n);
}


int main() {
	clientConn();
	char username[100], password[100];
	printf("\n Username :"); scanf("%s", username);
	printf("\n Password :"); scanf("%s", password);
	send(csd1, username, sizeof(username), 0);
	send(csd1, password, sizeof(password), 0);
	char inm[1000];
	recv(csd1, inm, sizeof(inm), 0);
	if(strcmp(inm, "invalid user") == 0) {
		printf("\n %s", inm);
		return 0;
	}
	while(1) {
		char command[1000];
		scanf("%s", command);
		if(strcmp(command, "list") == 0) {
			send(csd1, command, sizeof(command), 0);
			char files[10000];
			recv(csd1, files, sizeof(files), 0);
			printf("\n %s", files);						
		}
		else if(strcmp(command, "exit") == 0) {
			send(csd1, command, sizeof(command), 0);
			char mess[1000];
			recv(csd1, mess, sizeof(mess), 0);
			printf("\n %s", mess);
			close(csd1);
			return 1;
		}
		else {
			char exe[1000], filename[1000];
			int i = 0, j = 0, k = 0, f = 0;
			while(command[i] != '\0') {
				if(!f) {
					exe[j] = command[i];
					j++;
				}
				else if(command[i] == ' ') {
					exe[j] = '\0';
					f = 1;
				}
				else {
					filename[k] = command[i];
					k++;
				}
				i++;				
			} filename[k] = '\0';
			if(strcmp(exe, "delete") == 0) {
				send(csd1, command, sizeof(command), 0);
				char mess[1000];
				recv(csd1, mess, sizeof(mess), 0);
				printf("\n %s", mess);
			}
			else if(strcmp(exe, "store") == 0) {
				send(csd1, command, sizeof(command), 0);
				serverConn();
				FILE *fptr = fopen(filename, "r");
				char c, content[1000];
				int i = 0;
				while(fscanf(fptr, "%c", &c) != EOF) {
					content[i++] = c;
				}content[i] = '\0';
				send(ssd2, content, sizeof(content), 0);	
				close(ssd1);
				close(ssd2);			
			} 
			else if(strcmp(exe, "retrieve") == 0) {
				send(csd1, command, sizeof(command), 0);
				serverConn();
				char mess[5];
				recv(csd1, mess, sizeof(mess), 0);
				if(strcmp(mess, "00") == 0) {
					printf("\n Invalid file");
				}
				else {
					char filecontent[1000];
					recv(ssd2, filecontent, sizeof(filecontent), 0);
					FILE *fptr = fopen(filename, "w");
					fprintf(fptr, "%s", filecontent);
					fclose(fptr);
					printf("\n FIle written");
				}
				close(ssd1);close(ssd2);				
			} else {
				send(csd1, command, sizeof(command), 0);
				char messs[10000];
				recv(csd1, messs, sizeof(messs), 0);
				printf("\n %s", messs);
			}
		}
	}		
}
