/*
 * server.c
 * Copyright (C) 2013 suren <suren@bitbowl>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

int main(void)
{
    struct sockaddr_in s_addr, c_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = 3048;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server = socket(AF_INET, SOCK_STREAM, 0);
    bind(server, (struct sockaddr*)&s_addr, sizeof s_addr);
    listen(server, 1);
    printf("Server established...\n");
    while(1) {
        int n = sizeof c_addr;
        int client = accept(server, (struct sockaddr*)&c_addr, &n);
        printf("Connection established...\n");

        char file_name[20];
        recv(client, &file_name, sizeof file_name, 0);
        printf("File requested: %s\n", file_name);
        char buffer[2000];
        int handle = open(file_name, O_RDONLY);
        int s;
        if(handle == -1) {
            printf("File not found!\n");
            s = -1;
            send(client, &s, sizeof s, 0);
            send(client, "", sizeof buffer, 0);
            printf("Sent Empty string");
        }
        else {
            printf("File found!\n");
            read(handle, buffer, sizeof buffer);
            s = 0;
            send(client, &s, sizeof s, 0);
            send(client, &buffer, sizeof buffer, 0);
            printf("File sent.\n");
        }

        close(client);
    }
    close(server);
    return 0;
}
