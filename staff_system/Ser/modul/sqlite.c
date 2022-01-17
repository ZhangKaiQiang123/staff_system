#include "../inc/socket.h"
#include "../inc/sqlite.h"

static int create_form(sqlite3 * db) 
{
    char* err_msg = NULL;
    int ret = 0;
    char sql[SQL_LINE] = "create table if not exists info(Name char primary key, passwd char, phone char ,addr char, salary int, age int, identity char, state char);";
    ret = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if(ret != SQLITE_OK)
    {   
        fprintf(stderr, "sqlite_exec : %s line : %d\n", err_msg, __LINE__);
        return -1;
    }   
    return 0;
}
                                                                                          
int init_sqlite(sqlite3 ** db) 
{
    int ret = 0;
    char sql[SQL_LINE] = ""; 
    //1.打开数据库
    ret = sqlite3_open("./staff.db", db);
    if(ret != SQLITE_OK)
    {   
        fprintf(stderr, "sqlite_open: %s\n", sqlite3_errmsg(*db));
        return -1; 
    }   

    //2.创建表格
    ret = create_form(*db);
    if(ret < 0)
    {   
        printf("create_form error\n");
        return -1; 
    }   

    return 0;
}
