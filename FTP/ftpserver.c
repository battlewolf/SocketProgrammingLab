#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT1 8000
#define PORT2 9000

int csd1, ssd1, ssd2;
struct sockaddr_in s11, s12, c11;

void serverConn() {
	ssd1 = socket(PF_INET, SOCK_STREAM, 0);
	
	s11.sin_addr.s_addr = inet_addr("127.0.0.1");
	s11.sin_port = PORT1;
	s11.sin_family = PF_INET;
	
	bind(ssd1, (struct sockaddr *) &s11, sizeof(s11));
	listen(ssd1, 3);
	
	int n = sizeof(s12);
	ssd2 = accept(ssd1, (struct sockaddr *) &s12, &n);
}

void clientConn() {
	csd1 = socket(PF_INET, SOCK_STREAM, 0);
	
	c11.sin_addr.s_addr = inet_addr("127.0.0.1");
	c11.sin_port = PORT2;
	c11.sin_family = PF_INET;
	
	connect(csd1, (struct sockaddr *) &c11, sizeof(c11));	
}

void processListCommand() {
	char message[10000], c;
	printf("\n Client Requesting for listing");
	FILE *fptr = popen("ls", "r");	
	int i = 0;
	while(fscanf(fptr, "%c", &c) != EOF) {
		message[i++] = c;
	} message[i] = '\0';
	send(ssd2, message, sizeof(message), 0);	
	pclose(fptr);
}

void processDeleteCommand(char filename[]) {
	char command[10000], message[10000];
	strcpy(command, "rm ");
	strcat(command, filename);	
	printf("\n Client requestin for file delettion %s", filename);
	system(command);
	strcpy(message, "File not in existence anymore. . .");
	send(ssd2, message, sizeof(message), 0);	
}

void processStoreCommand(char filename[]) {
	clientConn();
	char filecontent[10000];
	recv(csd1, filecontent, sizeof(filecontent), 0);
	printf("\n Client Requesting to store a file %s", filename);
	FILE *fptr = fopen(filename, "w");
	fprintf(fptr, "%s", filecontent);
	fclose(fptr);
	close(csd1);
}

void processRetrieveCommand(char filename[]) {
	clientConn();
	printf("\n Client Requesting to retrieve a file %s", filename);
	char filecontent[10000], message[1000];
	FILE *fptr = fopen(filename, "r");
	if(fptr == NULL) {
		strcpy(message, "00");
		send(ssd2, message, sizeof(message), 0);
		close(csd1);
		return;
	}
	strcpy(message, "01");
	send(ssd2, message, sizeof(message), 0);
	char c; int i = 0;
	while(fscanf(fptr, "%c", &c) != EOF) {
		filecontent[i++] = c;
	} filecontent[i] = '\0';
	send(csd1, filecontent, sizeof(filecontent), 0);
	close(csd1);
}

void run() {
	char username[100], password[100];
	serverConn();
	recv(ssd2, username, sizeof(username), 0);
	recv(ssd2, password, sizeof(password), 0);
	printf("\n %s %s", username, password);
/*	if(strcmp(username, "archie") != 0 || strcmp(password, "betty") != 0) {*/
/*		char message[100];*/
/*		printf("\n FUck");*/
/*		strcpy(message, "invalid user");*/
/*		close(ssd1); close(ssd2);*/
/*		return ;*/
/*	}*/
	printf("\n Hello");
	char command[15000];
	while(1) {
		printf("\n Valid user");
		recv(ssd2, command, sizeof(command), 0);
		if(strcmp(command, "list") == 0) 
				processListCommand();
		else if(strcmp(command, "exit") == 0) {
			char message[1000];
			strcpy(message, "Bye closing conn");
			send(ssd2, message, sizeof(message), 0);
			close(ssd1); close(ssd2);
			return;
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
				processDeleteCommand(filename);
			}
			else if(strcmp(exe, "retrieve") == 0) {
				processRetrieveCommand(filename);
			}
			else if(strcmp(exe, "store") == 0) {
				processStoreCommand(filename);
			}			
			else {
				char message[1000];
				strcpy(message, "invalid command");
				send(ssd2, message, sizeof(message), 0);
			}
		}
	}
}

main() {
	run();		
}
