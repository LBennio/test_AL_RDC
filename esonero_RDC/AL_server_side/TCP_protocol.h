/*
 * TCP_protocol.c
 *
 * Created 19 nov 2024
 * Author: Linda Germinario
 */

#ifndef TCP_PROTOCOL_H_INCLUDED
#define TCP_PROTOCOL_H_INCLUDED

#define PROTOPORT 62001
#define BUFFER_SIZE 64
#define QLEN 5

void errorhandler(char *errorMessage);
void clearWinsock();

#endif // TCP_PROTOCOL_H_INCLUDED
