#include "net.h"
#include <arpa/inet.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER 1024

// inititalize connection
int conn_init(const char *ip_address, const uint16_t port) {
    int server_fd, sock;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd,
                   SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT,
                   &opt,
                   sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((sock = accept(server_fd,
                       (struct sockaddr *)&address,
                       (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    return sock;
}

// destroy connection
int conn_dest(const int sock) {
    close(sock);

    return 0;
}

char *serialize(const void *value, char *result, size_t size) {
    if (value == NULL) {
        return NULL;
    }

    // Using memcpy to handle binary data properly
    memcpy(result, value, size);

    return result;
}

int deserialize(struct Mesg msg, void *val) {
    if (msg.val == NULL || val == NULL) {
        return -1;
    }

    memcpy(val, msg.val, ntohs(msg.len));
    // null-terminate the string if it's a character array
    // TODO this probably isnt needed
    // if (msg.len > 0 && ((char *)val)[msg.len - 1] == '\0') {
    //    ((char *)val)[msg.len - 1] = '\0';
    //}

    return 0;
}

void publish(const int sock, uint8_t type, size_t len, const void *val) {
    // message struct
    struct Mesg msg;

    msg.type = type;
    msg.len = htons(len);
    memcpy(msg.val, val, len);

    send(sock, &msg, sizeof(struct Mesg), 0);
    // printf("Data sent - type: %d, len: %d\n", type, len);
}

int sub_init(const char *ip_address, const uint16_t port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024]; // = {0};

    // creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void subscribe(const char *ip_address, const uint16_t port) {
    // initialize subscribe
    int sock = sub_init(ip_address, port);
    // client struct
    struct sockaddr_in client;
    int valread, c;
    size_t len;
    char buffer[BUFFER]; // = {0};
    struct Mesg msg;

    while (1) {
        // len = sizeof(client);
        //  receive the message
        //  valread = read(sock, buffer, sizeof(buffer));
        valread = recv(sock, &msg, sizeof(msg), 0);

        if (valread <= 0) {
            // error with socket connection
            perror("Connection closed or error");
            break;
        }

        uint8_t type = msg.type;
        len = ntohs(msg.len);

        // allocate memory for the received data
        void *val = malloc(len);
        if (val == NULL) {
            perror("ERROR: Unable to allocate memory");
            break;
        }

        if (deserialize(msg, val) == 0) {
            switch (type) {
            case T_DOUBLE:
                printf("Received double: %0.4lf\n", *((double *)val));
                break;
            case T_INT32:
                printf("Received int32_t: %d\n", *((int32_t *)val));
                break;
            case T_INT64:
                printf("Received int64_t: %ld\n", *((int64_t *)val));
                break;
            case T_FLOAT:
                printf("Received float: %0.4f\n", *((float *)val));
                break;
            case T_CHAR:
                printf("Received char array: %s\n", (char *)val);
                break;
            default:
                printf("ERROR: Unable to determine type - type: %d, len: %ld\n",
                       type,
                       len);
                break;
            }
        }

        // free the allocated memory and wipe buffer
        free(val);
        memset(&msg, 0, sizeof(msg));
        // memset(buffer, 0, sizeof(buffer));
        //  10ms delay or else empty char array is printed after previous
        //  transmission
        usleep(10000);
    }

    close(sock);
}
