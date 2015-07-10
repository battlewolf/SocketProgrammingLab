
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int validate(char* cmd, char* key, char* param) {
    int i;
    for(i = 0; i < 4; i++) {
        if(cmd[i] <'A' || cmd[i] > 'Z')
            return -1;
        key[i] = cmd[i];
    }
    if(cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\0')
        return -1;
    key[i] = '\0';
    while(cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\0') i++;
    int j = 0;
    while(cmd[i] != '\0') {
        param[j++] = cmd[i++];
    }
    param[j] = '\0';
    return 0;
}
int main(void)
{
    struct sockaddr_in s_addr, c_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = 3048;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof reuse) == 1)
        printf("Error\n");
    bind(server, (struct sockaddr*)&s_addr, sizeof s_addr);
    listen(server, 1);
    printf("Server established...\n");
    int n = sizeof c_addr;
    int client = accept(server, (struct sockaddr*)&c_addr, &n);
    printf("Connection established...\n");

    char cmd[100] = "";
    int status;
    char key[5], param[80];
    int session = 0;
    while(strcmp(cmd, "EXIT") != 0) {
        recv(client, cmd, sizeof cmd, 0);
        printf("Received: %s\n", cmd);
        status = validate(cmd, key, param);
        printf("Status: %d\n", status);
        printf("key: %s\n", key);
        printf("param: %s\n", param);
        if(status == 0) {
            switch(session) {
                case 0:
                    if(strcmp(key, "USER") == 0) {
                        if(strcmp(param, "admin") == 0)
                            session = 1;
                        else {
                            status = 1;
                        }
                    }
                    else
                        status = -1;
                    send(client, &status, sizeof status, 0);
                    break;
                case 1: 
                    if(strcmp(key, "PASS") == 0) {
                        if(strcmp(param, "admin") == 0)
                            session = 2;
                        else {
                            status = 2;
                            session = 0;
                        }
                    }
                    else 
                        status = -1;
                    send(client, &status, sizeof status, 0);
                    break;
                case 2:
                    if(strcmp(key, "LIST") == 0) {
                        FILE *fp = popen("ls", "r");
                        char buff[100];
                        status = 10;
                        send(client, &status, sizeof status, 0);
                        while(fgets(buff, sizeof buff, fp) != NULL){
                            send(client, buff, sizeof buff, 0);
                        }
                        send(client, "STOP\0", 5, 0);
                    }
                    else if(strcmp(key, "RETR") == 0) {
                        status = 11;
                        send(client, &status, sizeof status, 0);

                        struct sockaddr_in cdip;
                        cdip.sin_family = AF_INET;
                        cdip.sin_port = 30481;
                        cdip.sin_addr.s_addr = inet_addr("127.0.0.1");
                        int datacli = socket(AF_INET, SOCK_STREAM, 0);
                        sleep(1);
                        int temp = connect(datacli, (struct sockaddr*)&cdip, sizeof cdip);
                        printf("Data connection established: %d\n", temp);

                        FILE *fp = fopen(param, "r");
                        char buff[100];
                        while(fgets(buff, sizeof buff, fp) != NULL){
                            send(datacli, buff, sizeof buff, 0);
                        }
                        send(datacli, "STOP\0", 5, 0);
                        close(datacli);

                    }
                    else if(strcmp(key, "STOR") == 0) {
                        status = 12;
                        send(client, &status, sizeof status, 0);

                        struct sockaddr_in cdip;
                        cdip.sin_family = AF_INET;
                        cdip.sin_port = 30481;
                        cdip.sin_addr.s_addr = inet_addr("127.0.0.1");
                        int datacli = socket(AF_INET, SOCK_STREAM, 0);
                        sleep(1);
                        int temp = connect(datacli, (struct sockaddr*)&cdip, sizeof cdip);
                        printf("Data connection established: %d\n", temp);

                        FILE *fp = fopen(param, "w+");
                        char buff[100];
                        recv(datacli, buff, sizeof buff, 0);
                        while(strcmp(buff, "STOP\0") != 0) {
                            printf("%s", buff);
                            fputs(buff, fp);
                            recv(datacli, buff, sizeof buff, 0);
                        }
                        fclose(fp);
                        close(datacli);
                    }
                    else if(strcmp(key, "DELE") == 0) {
                        status = 13;
                        send(client, &status, sizeof status, 0);
                        char s[100] = "rm ";
                        strcat(s, param);
                        system(s);
                    }
                    else {
                        status = -1;
                        send(client, &status, sizeof status, 0);
                    }
                    break;

            }
        }
        else
            send(client, &status, sizeof status, 0);
    }

    close(client);
    close(server);
    return 0;
}
