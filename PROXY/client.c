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

int main(void)
{
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = 30481;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server = socket(AF_INET, SOCK_STREAM, 0);
    connect(server, (struct sockaddr*)&s_addr, sizeof s_addr);
    printf("Connection established...\n");

    char file_name[20];
    printf("Enter filename to be fetched: ");
    scanf("%s", file_name);
    send(server, &file_name, sizeof file_name, 0);
    char buffer[2000];
    int s;
    recv(server, &s, sizeof s, 0);
    recv(server, &buffer, sizeof buffer, 0);
    if(s != -1) {
        printf("\n***********************************\n");
        printf("%s", buffer);
        printf("\n***********************************\n");
    }
    else
        printf("File not found!\n");
    close(server);
    return 0;

}
