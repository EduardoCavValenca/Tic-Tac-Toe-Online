#ifndef COMMON_H
#define COMMON_H
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <netinet/in.h>

#define SERVER_PORT 17000
#define BUFFER_SIZE 256

typedef struct sockaddr SA;
typedef struct sockaddr_in SAIN;

using namespace std;

int check (int exp, const char* msg);
void printMatriz3x3(char m[3][3]);

#endif