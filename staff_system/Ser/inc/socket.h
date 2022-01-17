#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>                                                                    
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENTS 1024
#define NAME_LEN   20
#define PASSWD_LEN 20
#define PHONE_LEN  15
#define ADDR_LEN   64

typedef struct {
    char cmd;
    char name[NAME_LEN];
    char passwd[PASSWD_LEN];
    char phone[PHONE_LEN];
    char addr[ADDR_LEN];
    int age;
    float salary;
    char identity;
    char state;
}__attribute__ ((__packed__)) package; 

#define PACK_LEN sizeof(package)
  
#define ERR_MSG(msg) do{\
    perror(msg);\
    printf("%s : %s : %d \n", __FILE__, __func__, __LINE__);\
}while(0)

int init_socket(const char* ip, const short port, struct sockaddr_in* sin);

#endif
