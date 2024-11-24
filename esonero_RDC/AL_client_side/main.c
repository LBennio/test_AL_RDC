/*
 * main.c
 *
 * Created 20 nov 2024
 * Author: Ennio Lo Buono
 */

#include "TCP_protocol.h"
#include "checks.h"

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif // defined

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void errorhandler(char *errorMessage) {
	printf("%s\n", errorMessage);
}

void clearWinsock() {

#if defined WIN32
	WSACleanup();
#endif
}

int main(void)
{
    #if defined WIN32
        WSADATA wsa_data;
        WORD version_requested;

        version_requested = MAKEWORD(2, 2);
        int result = WSAStartup(version_requested, &wsa_data);

        if(result != 0) {
            printf("Error at WSAStartup()\n");
            printf("no usable winsock DLL found\n");
            return -1;
        }

    #endif // defined

    int c_socket;

    c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_socket < 0) {

		errorhandler("Socket creation failed. \n");
		clearWinsock();
		return -1;
	}

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));

	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(SOCKET_ADDRESS);
	sad.sin_port = htons(PROTOPORT);

	if(connect(c_socket, (struct sockaddr*)&sad, sizeof(sad)) < 0) {
        errorhandler("Failed to connect. \n");
		closesocket(c_socket);
		clearWinsock();
		return -1;
	}

	// receives server data
    int bytes_rcvd;
    char buf[BUFFER_SIZE]; // buffer containing data from the server
    memset(&bytes_rcvd, 0, BUFFER_SIZE);  // ensure extra bytes contain 0

    printf("waiting for connection --> ");

    if ((bytes_rcvd = recv(c_socket, buf, BUFFER_SIZE - 1, 0)) <= 0)
		{
		errorhandler("recv() failed or connection closed prematurely");
		system("pause");
		closesocket(c_socket);
		clearWinsock();
		return -1;
	}

	// prints out the connection acceptance message from the server
	buf[bytes_rcvd] = '\0';
	printf("%s", buf);
	system("pause");
	system("cls");

	// ------------------------------------------- CONNECTION

	// string used to send messages to the server
	char string_send[BUFFER_SIZE];
	memset(string_send, '\0', BUFFER_SIZE);  // ensures extra bytes are set to '\0'

	printf("%s\n%s\n",
        "insert a command to produce a password of size between 6 and 32",
        "format: 'type[space]length'");

    gets(string_send);

    // checks the validity of the string that's just read
    while(!string_check(string_send)) {         // loop ending only when a valid string is inserted
        system("cls");
        printf("Invalid input: %s\n", string_send);

        printf("%s\n%s\n",
            "insert a command to produce a password of size between 6 and 32",
            "format: 'type[space]length'");

        gets(string_send);
    }

    // send the string to the server
    int string_len = strlen(string_send);

    if(send(c_socket, string_send, string_len, 0) != string_len) {
        errorhandler("send() sent a different number of bytes than expected");
		closesocket(c_socket);
		clearWinsock();
		return -1;
    }

    char result_in[BUFFER_SIZE]; // buffer for data from the server

    // loop executing until the terminating command "q" is inserted
    while(strcmp(string_send, "q") != 0) {

        memset(result_in, '\0', BUFFER_SIZE);   // cleans the buffer for the incoming message from the server

		printf("Result: ");


		if ((bytes_rcvd = recv(c_socket, result_in, BUFFER_SIZE - 1, 0)) <= 0) {
			errorhandler("recv() failed or connection closed prematurely");
			closesocket(c_socket);
			clearWinsock();
			return -1;
		}

		printf("%s\n", result_in);  // print the result from the operation
		system("pause");
        system("cls");

		memset(string_send, '\0', BUFFER_SIZE); // clean up the buffer for the messages to send

        printf("%s\n%s\n",
            "insert a command to produce a password of size between 6 and 32",
            "format: 'type[space]length'");
        gets(string_send);

        while(!string_check(string_send)) {  // loop ending only when a valid string is inserted
		    system("cls");
            printf("%s: %s\n", "Invalid input", string_send);

            printf("%s\n%s\n",
                "insert a command to produce a password of size between 6 and 32",
                "format: 'type[space]length'");

            gets(string_send);
        }

        // sending the string to the server
        string_len = strlen(string_send);

		if (send(c_socket, string_send, string_len, 0) != string_len) {
			errorhandler("send() sent a different number of bytes than expected");
			closesocket(c_socket);
			clearWinsock();
			return -1;
		}
    }

    system("pause");

    // closing the connection
	closesocket(c_socket);
	clearWinsock();
	printf("\n");

    return 0;
}
