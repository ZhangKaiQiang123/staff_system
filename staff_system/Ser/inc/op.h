#ifndef __OP_H__
#define __OP_H__

#include "socket.h"
#include "sqlite.h"
#include "menu.h"

int do_register(int fd, package* pack, sqlite3* db);
int do_login(int fd, package* pack, sqlite3* db);
int do_quit(int fd, package *pack, sqlite3* db);
int add_worker(int fd, package *pack, sqlite3* db);
int del_worker(int fd, package *newpack, sqlite3* db);
int search_worker(int fd, package *newpack, sqlite3* db);
int modify_worker(int fd, package *newpack, sqlite3* db);
int list_worker(int fd, package *newpack, sqlite3* db);

#endif