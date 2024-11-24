/*
 * main.c
 *
 * Created 19 nov 2024
 * Author: Linda Germinario
 */

#include "TCP_protocol.h"
#include "password_generation.h"
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void errorhandler(char *errorMessage) {

	printf("%s", errorMessage);
}

void clearWinsock() {

#if defined WIN32
	WSACleanup();
#endif
}


int main(int argc, char *argv[]) {
    int port;

    // Check if a port argument is provided
    if(argc > 1) {
        port = atoi(argv[1]);  // Convert argument to integer if provided
    } else {
        port = PROTOPORT;      // Use default port if none is provided
    }

    #if defined WIN32
    // Windows-specific initialization for Winsock
    WSADATA wsa_data;
    WORD version_requested = MAKEWORD(2,2); // Request Winsock version 2.2
    int result = WSAStartup(version_requested, &wsa_data);

    // Check if WSAStartup succeeded
    if(result != NO_ERROR) {
        printf("Error at WSAStartup() \n");
        printf("A usable WinSock DLL cannot be found\n");
        return 0;
    }
    #endif

    // Create a socket
    int my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(my_socket < 0) {
        errorhandler("Socket creation failed.\n");
        clearWinsock();
        return -1;
    }

    // Define server address structure
    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));    // Zero out structure
    sad.sin_family = AF_INET;        // Set family to Internet
    sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set IP address to localhost
    sad.sin_port = htons(port);      // Set port number

    // Bind the socket to the address and port
    if(bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
        errorhandler("bind() failed.\n");
        closesocket(my_socket);
        return -1;
    }

    // Set the socket to listen for incoming connections
    if(listen(my_socket, QLEN) < 0) {
        errorhandler("listen() failed.\n");
        system("pause");
        closesocket(my_socket);
        clearWinsock();
        return -1;
    }

    struct sockaddr_in cad; // Client address structure
    int client_socket;      // Socket descriptor for client
    int client_len;         // Size of client address structure

    // Loop to keep the server listening for connections
    while(1) {
        system("cls");
        printf("Waiting for a client to connect...\n");

        client_len = sizeof(cad);

        // Accept a client connection
        if((client_socket = accept(my_socket, (struct sockaddr*) &cad, &client_len)) < 0) {
            errorhandler("accept() failed.\n");
            system("pause");
            closesocket(my_socket);
            clearWinsock();
            return -1;
        }

        printf("\nConnection established with %s:%d\n", inet_ntoa(cad.sin_addr), ntohs(cad.sin_port));

        // Send "Connection established" message to the client
        char* input_string = "Connection established\n";
        int string_len = strlen(input_string);
        if (send(client_socket, input_string, string_len, 0) != string_len) {
            errorhandler("send() sent a different number of bytes than expected\n");
            system("pause");
            closesocket(client_socket);
            clearWinsock();
            return -1;
        }

        // Buffer for receiving data from the client
        int bytes_rcvd;
        char str_in[BUFFER_SIZE];
        memset(str_in, '\0', BUFFER_SIZE); // Initialize buffer with null characters
        printf("Received: ");

        // Receive the first message from the client
        if ((bytes_rcvd = recv(client_socket, str_in, BUFFER_SIZE - 1, 0)) <= 0) {
            printf("Error receiving from client.\n");
            continue;  // Continue the loop if receiving fails
        }

        str_in[bytes_rcvd] = '\0';  // Null-terminate received data
        fprintf(stdout, "%s\n", str_in);  // Print received data

        // Loop to keep receiving messages from the client until "q" is received
        while(strcmp(str_in,"q") != 0) {
            // Variables for password generation
            char op;
            char result_out[100];
            int len = 0;

            memset(result_out, '\0', BUFFER_SIZE); // Initialize result buffer

            // Set the password generation parameters based on client input
            set_generation(&op, &len, str_in);
            password_generation(result_out, op, len);  // Generate password

            string_len = strlen(result_out);

            printf("Result: %s\n", result_out);

            // Send the generated password back to the client
            if (send(client_socket, result_out, string_len, 0) != string_len) {
                errorhandler("\nsend() sent a different number of bytes than expected\n");
                system("pause");
                closesocket(client_socket);
                clearWinsock();
                return -1;
            }

            // Receive a new string from the client
            if ((bytes_rcvd = recv(client_socket, str_in, BUFFER_SIZE - 1, 0)) <= 0) {
                errorhandler("\nrecv() failed or connection closed prematurely\n");
                break;  // Exit the loop if receiving fails
            }
            printf("Received: %s\n", str_in); // Print the received string
            str_in[bytes_rcvd] = '\0'; // Null-terminate received data
        }
    }
}
