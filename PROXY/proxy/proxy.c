/*
 * proxy.c
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
    s_addr.sin_port = 30481;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof reuse) == 1)
        printf("Error\n");
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
        if(handle == -1) {
            printf("File not found!\n");
            struct sockaddr_in p_addr;
            p_addr.sin_family = AF_INET;
            p_addr.sin_port = 3048;
            p_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

            int main = socket(AF_INET, SOCK_STREAM, 0);
            connect(main, (struct sockaddr*)&p_addr, sizeof p_addr);
            printf("Connected to main server!\n");

            send(main, file_name, sizeof file_name, 0);
            int s;
            recv(main, &s, sizeof s, 0);
            recv(main, buffer, sizeof buffer, 0);
            if(s == -1) {
                printf("File not present in main server!\n");
            }
            else {
                FILE *newf = fopen(file_name, "w");
		        fputs(buffer,newf);
		        fclose(newf);
                //int x = write(newf, buffer, sizeof buffer);
                //printf("w: %d\n", x);
            }
            send(client, &s, sizeof s, 0);
            send(client, buffer, sizeof buffer, 0);
        }
        else {
            printf("File found!\n");
            int s = 1;
            read(handle, buffer, sizeof buffer);
            send(client, &s, sizeof s, 0);
            send(client, &buffer, sizeof buffer, 0);
        }
        printf("File sent.\n");
        close(client);
    }
    close(server);
    return 0;
}
