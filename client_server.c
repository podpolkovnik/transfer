#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client_server.h"

void* clientHandler(void* arg) {
    handler_args args = *((handler_args*)arg);
    packet msg;

    clock_t timer = args.time * CLOCKS_PER_SEC;
    clock_t before = clock();

    while (1) {
        if (clock() - before >= timer) {
            struct timespec time_val;

            strncpy(msg.message, args.message, sizeof(args.message));
            clock_gettime(CLOCK_REALTIME, &time_val);
            msg.time_val = time_val;

            if (send(args.client_sock_fd, &msg, sizeof(msg), 0) == -1)
                break;
            before = clock();
        }
    }

    close(args.client_sock_fd);
    pthread_exit(NULL);
}

void run_server(int socket_fd, struct sockaddr_in server_address, char message[MSG_LEN], size_t time) {
    if (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Socket bind error\n");
        return;
    }

    if (listen(socket_fd, 50) == -1) {
        perror("Socket listen error\n");
        close(socket_fd);
        return;
    }

    pthread_t tid[51];
    int i = 0;
    handler_args args;

    while (1) {
        int client_socket_fd = accept(socket_fd, NULL, NULL);
        args.client_sock_fd = client_socket_fd;
        strncpy(args.message, message, sizeof(args.message));
        args.time = time;

        if (client_socket_fd == -1) {
            perror("Accept socket error\n");
            break;
        }
        
        if(pthread_create(&tid[i++], NULL, clientHandler, &args) != 0) {
            perror("Thread create error\n");
            break;
        }

        if(i >= 50) {
            i = 0;
            while(i < 50) {
                pthread_join(tid[i++], NULL);
            }
        }
    }
    close(socket_fd);
}


void run_client(int socket_fd, struct sockaddr_in server_address) {
    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Socket connection error\n");
        return;
    }

    while (true) {
        packet server_response;
        struct timespec time_val;
        struct timespec recv_time_val;

        ssize_t bytes = recv(socket_fd, &server_response, sizeof(server_response), 0);

        if (bytes <= 0) {
            break;
        }

        clock_gettime(CLOCK_REALTIME, &time_val);
        recv_time_val.tv_sec = time_val.tv_sec - server_response.time_val.tv_sec;
        recv_time_val.tv_nsec = time_val.tv_nsec - server_response.time_val.tv_nsec;

        printf("\nThe server sent the data:\n");
        printf("    message: %s\n", server_response.message);
        printf("    timestamp: %d.%d\n", server_response.time_val.tv_sec, server_response.time_val.tv_nsec);
        printf("    recv time: %d sec, %d nsec\n", recv_time_val.tv_sec, recv_time_val.tv_nsec);
    }

	close(socket_fd);
}

void run(const run_params* params) {
    int socket_fd;
    struct sockaddr_in server_address;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

    const int enable = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("Set socket option SO_REUSEADDR failed\n");

    switch(params->run_mode) {
        case RUN_MODE_CLIENT:
            run_client(socket_fd, server_address);
            break;
        case RUN_MODE_SERVER:
            run_server(socket_fd, server_address, params->message, params->time);
            break;
        default:
            perror("Error, unknown or undefined run mode type\n");
    }
}