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

    printf("Created new socket: %d\n", sock);
    return sock;
}

// destroy connection
int conn_dest(const int sock) {
    close(sock);

    return 0;
}

/*void publish(const int sock, const char *message) {
    send(sock, message, strlen(message), 0);
    printf("Message sent:   %s\n", message);
}*/

char *serialize(const void *value, char *result, size_t size) {
    if (value == NULL) {
        return NULL;
    }

    // Using memcpy to handle binary data properly
    memcpy(result, value, size);

    return result;
}

int deserialize(const char *data, void *result, size_t size) {
    if (data == NULL || result == NULL) {
        return -1;
    }

    memcpy(result, data, size);
    // null-terminate the string if it's a character array
    // TODO this probably isnt needed
    if (size > 0 && ((char *)result)[size - 1] == '\0') {
        ((char *)result)[size - 1] = '\0';
    }

    return 0;
}

void publish(const int sock, const void *data, size_t size) {
    // message struct
    struct Mesg msg;

    // convert data to string
    char ser_data[BUFF_SZ]; //= serialize()

    serialize(data, ser_data, size);

    msg.size = htons(size);
    memcpy(msg.data, ser_data, size);

    printf("size of: %ld", sizeof(data));

    send(sock, &msg, sizeof(struct Mesg), 0);

    printf("Data sent.\n");
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
    int valread, len, c;
    char buffer[BUFFER]; // = {0};
    struct Mesg msg;

    while (1) {
        len = sizeof(client);
        // receive the message
        // valread = read(sock, buffer, sizeof(buffer));
        valread = recv(sock, &msg, sizeof(struct Mesg), 0);

        if (valread <= 0) {
            // error with socket connection
            perror("Connection closed or error");
            break;
        }

        size_t data_sz = ntohs(msg.size);

        // allocate memory for the received data
        void *received_data = malloc(data_sz);
        if (received_data == NULL) {
            perror("Memory allocation error");
            break;
        }

        // deserialize the data
        if (deserialize(msg.data, received_data, data_sz) == 0) {
            // handling every type is probably too tedious
            if (data_sz == sizeof(double)) {
                printf("Received double: %lf\n", *((double *)received_data));
            } else if (data_sz == sizeof(int32_t)) {
                printf("Received int32_t: %d\n", *((int32_t *)received_data));
            } else if (data_sz == sizeof(float)) {
                printf("Received float: %f", *((float *)received_data));
            } else {
                printf("Received char array: %s\n", (char *)received_data);
            }
        } else {
            perror("Deserialization error");
        }

        // free the allocated memory and wipe buffer
        free(received_data);
        memset(buffer, 0, sizeof(buffer));
        // 10ms delay or else empty char array is printed after previous
        // transmission
        usleep(10000);
    }

    close(sock);
}
