#ifndef __OP_H__
#define __OP_H__
#include "socket.h"


int do_register(int sfd);
int do_login(int sfd, package* pack);
int do_quit(int sfd, package* pack);
int add_worker(int sfd);
int del_worker(int sfd);
int search_worker(int sfd);
int modify_worker(int sfd);
int list_worker(int sfd);

int search_self(int sfd, package* login_pack);
int modify_self(int sfd, package* login_pack);

int login_operation_quit(int fd);

#endif