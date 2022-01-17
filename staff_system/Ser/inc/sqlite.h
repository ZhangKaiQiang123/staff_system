#ifndef __SQLITE_H__
#define __SQLITE_H__

#include "socket.h"
#include <sqlite3.h>

#define SQL_LINE 256
int init_sqlite(sqlite3 ** db);                                                                                   

#endif