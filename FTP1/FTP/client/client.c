/*
 * client.c
 * Copyright (C) 2013 suren <suren@bitbowl>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
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
    struct sockaddr_in s_addr;
    struct sockaddr_in sdip, cdip;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = 3048;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server = socket(AF_INET, SOCK_STREAM, 0);
    connect(server, (struct sockaddr*)&s_addr, sizeof s_addr);
    printf("Connection established...\n");

    char cmd[100] = "";
    int status;
    while(1) {
        printf("Command :");
        fgets(cmd, sizeof cmd, stdin);
        if(strcmp(cmd, "\n") == 0) continue;
        int i = -1;
        char buff[100] = "";
        while(cmd[++i] != '\n');
        cmd[i] = '\0';
        send(server, cmd, sizeof cmd, 0);
        recv(server, &status, sizeof status, 0);
        /*printf("Status: %d\n", status);*/
        int dataserv, reuse, bx, nn, datacli;
        if(strcmp(cmd, "EXIT") == 0)
            break;
        switch (status) {
            case -1:
                printf("Invalid command\n");
                break;
            case 1:
                printf("Wrong username\n");
                break;
            case 2:
                printf("Wrong password\n");
                break;
            case 10:
                recv(server, buff, sizeof buff, 0);
                while(strcmp(buff, "STOP\0") != 0) {
                    printf("%s", buff);
                    recv(server, buff, sizeof buff, 0);
                    /*printf("%s", buff);*/
                }
                break;
            case 11:
                sdip.sin_family = AF_INET;
                sdip.sin_port = 30481;
                sdip.sin_addr.s_addr = inet_addr("127.0.0.1");
                dataserv = socket(AF_INET, SOCK_STREAM, 0);
                reuse = 1;
                if(setsockopt(dataserv, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof reuse) == 1)
                    printf("Error\n");
                bx = bind(dataserv, (struct sockaddr*)&sdip, sizeof sdip);
                /*printf("bind: %d\n", bx);*/
                listen(dataserv, 1);
                /*printf("Server established...\n");*/
                nn = sizeof cdip;
                datacli = accept(dataserv, (struct sockaddr*)&cdip, &nn);
                /*printf("Data connection established...\n");*/

                recv(datacli, buff, sizeof buff, 0);
                while(strcmp(buff, "STOP\0") != 0) {
                    printf("%s", buff);
                    recv(datacli, buff, sizeof buff, 0);
                    /*printf("%s", buff);*/
                }
                close(dataserv);
                close(datacli);
                break;
            case 12:
                sdip.sin_family = AF_INET;
                sdip.sin_port = 30481;
                sdip.sin_addr.s_addr = inet_addr("127.0.0.1");
                dataserv = socket(AF_INET, SOCK_STREAM, 0);
                reuse = 1;
                if(setsockopt(dataserv, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof reuse) == 1)
                    printf("Error\n");
                bx = bind(dataserv, (struct sockaddr*)&sdip, sizeof sdip);
                /*printf("bind: %d\n", bx);*/
                listen(dataserv, 1);
                /*printf("Server established...\n");*/
                nn = sizeof cdip;
                datacli = accept(dataserv, (struct sockaddr*)&cdip, &nn);
                /*printf("Data connection established...\n");*/

                char param[80], key[5];
                validate(cmd, key, param); 
                FILE *fp = fopen(param, "r");
                char buff[100];
                while(fgets(buff, sizeof buff, fp) != NULL){
                    /*printf("%s", buff);*/
                    int sx = send(datacli, buff, sizeof buff, 0);
                    /*printf("Send: %d\n", sx);*/
                }
                send(datacli, "STOP\0", 5, 0);

                fclose(fp);
                close(dataserv);
                close(datacli);
                break;
            case 13:
                printf("Deleted!\n");
                break;
        }
    }

    close(server);
    return 0;

}
