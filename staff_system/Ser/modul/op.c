#include "../inc/op.h"

static int do_search_name(sqlite3 *db, char* name, char identity, int *row, int *col, char*** result)
{
    char sql[SQL_LINE] = "";
    char *errmsg;
    sprintf(sql, "select * from info where name=\"%s\" and identity=\'%c\';",name, identity);
    printf("sql : %s\n", sql);
    if(sqlite3_get_table(db, sql, result, row, col, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        return -1;
    }
    return 0;
}

int do_register(const int fd, package* pack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char *errmsg;
    int ret;
    char buf[32];
    sprintf(sql, "insert into info values(\"%s\", \"%s\", \"%s\",\"%s\", %.2f, %d, \"%c\", \"%c\")", \
        pack->name, pack->passwd, pack->phone ,pack->addr, pack->salary, pack->age, pack->identity, pack->state);
    // printf("%c %s %s %s %d %c %c\n", pack->type,pack->name, 
    // pack->passwd, pack->id,pack->age, pack->identity, pack->state);
    printf("%s\n", sql);
    bzero(buf, sizeof(buf));
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        strcpy(buf, "failed");
    }
    else
    {
        strcpy(buf, "successful");
    }
    printf("buf : %s\n", buf);

    //回应ACK
    ret = send(fd, buf, sizeof(buf), 0);
    if(ret < 0)
    {
        ERR_MSG("send");
        return -1;
    }

    return 0;
}

int do_login(int fd, package* pack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char *errmsg;
    int ret;
    char buf[32];
    int row, col;
    char **result;
    int i=0;

    //查看表格中是否有此用户
    do_search_name(db, pack->name, pack->identity, &row, &col, &result);
    if(row != 1)
    {
        printf("name is not in\n");
        strcpy(buf, "notin");
        goto ERR1;
    }

    if(strcmp(result[9], pack->passwd) != 0)
    {
        strcpy(buf, "nopasswd");
        goto ERR1;
    }
    printf("result[15][0] = %c\n",result[15][0]);

    //判断是否重复登录
    if(result[15][0] != 'N')
    {
        strcpy(buf, "nostate");
        goto ERR1;
    }
    strcpy(buf, "success");
    //更改状态
    sprintf(sql, "update info set state='Y' where name='%s'", pack->name);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        return -1;
    }
    printf("sql = %s\n", sql);
ERR1:
    if(pack->identity == 'Y')
    {
        strcat(buf, "Y");
    }
    else if(pack->identity == 'N')
    {
        strcat(buf, "N");
    }
    else
    {
        strcat(buf, "M");
    }
    printf("buf : %s\n", buf);
    ret = send(fd, buf, sizeof(buf), 0);
    if(ret < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    return 0;
}

int do_quit(int fd, package *pack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char buf[32] = "";
    char *errmsg;
    int ret = 0;

    sprintf(sql, "update info set state='N' where name='%s'", pack->name);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        strcpy(buf, "error");
    }
    else
    {
        strcpy(buf, "success");
        printf("quit successful\n");
    }
    printf("quit worker result : %s\n", buf);
    if(send(fd, buf, sizeof(buf), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    return 0;

}

int add_worker(int fd, package *newpack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char buf[32] = "";
    char *errmsg;
    int ret = 0;

    sprintf(sql, "insert into info values(\"%s\", \"%s\", \"%s\",\"%s\", %.2f, %d, \"%c\", \"%c\")", \
        newpack->name, newpack->passwd, newpack->phone ,newpack->addr, newpack->salary, newpack->age,
         newpack->identity, newpack->state);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        strcpy(buf, "error");
    }
    else
    {
        strcpy(buf, "success");
        printf("insert successful\n");
    }
    printf("add worker result : %s\n", buf);
    if(send(fd, buf, sizeof(buf), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    printf("send buf: %s\n", buf);
    return 0;
}

int del_worker(int fd, package *newpack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char buf[32] = "";
    char *errmsg;
    int ret = 0;

    sprintf(sql, "delete from info where name=\"%s\"", newpack->name);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        strcpy(buf, "error");
    }
    else
    {
        strcpy(buf, "success");
        printf("delete successful\n");
    }

    if(send(fd, buf, sizeof(buf), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    return 0;
}

// static int do_search_name(sqlite3 *db, char* name, char identity, int *row, int *col, char*** result)
// {
//     char sql[SQL_LINE] = "";
//     char *errmsg;
//     sprintf(sql, "select * from info where name=\"%s\" and identity=\'%c\';",name, identity);
//     printf("sql : %s\n", sql);
//     if(sqlite3_get_table(db, sql, result, row, col, &errmsg) != SQLITE_OK)
//     {
//         printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
//         return -1;
//     }
//     return 0;
// }

int search_worker(int fd, package *newpack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char buf[32] = "";
    char **result; 
    int row=0, col=0;
    char *errmsg;
    int ret = 0;
    int i;


    sprintf(sql, "select * from info where name=\"%s\";",newpack->name);
    if(sqlite3_get_table(db, sql, &result, &row, &col, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        return -1;
    }
    if(send(fd, &row, sizeof(int), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    if(send(fd, &col, sizeof(int), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    for(i=0; i<col*(row+1); i++)
    {
        send(fd, result[i], 32, 0);
    }
}

int modify_worker(int fd, package *newpack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char buf[32] = "";
    char *errmsg;
    int ret = 0;

    int op;
    if(recv(fd, &op, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    printf("mop = %d\n", op);
    switch(op)
    {
        case 1:
            sprintf(sql, "update info set name=\"%s\" where phone=\"%s\" and addr=\"%s\";", newpack->name, newpack->phone, newpack->addr);
            break;
        case 2:
            sprintf(sql, "update info set passwd=\"%s\" where name=\"%s\";", newpack->passwd, newpack->name);
            break;
        case 3:
            sprintf(sql, "update info set phone=\"%s\" where name=\"%s\";", newpack->phone, newpack->name);
            break;
        case 4:
            sprintf(sql, "update info set addr=\"%s\" where name=\"%s\";", newpack->addr, newpack->name);
            break;
        case 5:
            sprintf(sql, "update info set salary=\"%.2f\" where name=\"%s\";", newpack->salary, newpack->name);
            break;
        case 6:
            sprintf(sql, "update info set age=\"%d\" where name=\"%s\";", newpack->age, newpack->name);
            break;
        case 0:
            return 0;
        default:
        printf("input error\n");
            break;
    }

    printf("sql : %s\n", sql);

    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        strcpy(buf, "error");
    }
    else
    {
        strcpy(buf, "success");
        printf("modify successful\n");
    }

    if(send(fd, buf, sizeof(buf), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    printf("buf : %s\n", buf);
}

int list_worker(int fd, package *newpack, sqlite3* db)
{
    char sql[SQL_LINE] = "";
    char buf[32] = "";
    char **result; 
    int row=0, col=0;
    char *errmsg;
    int ret = 0;
    int i;


    sprintf(sql, "select * from info;");
    if(sqlite3_get_table(db, sql, &result, &row, &col, &errmsg) != SQLITE_OK)
    {
        printf("sqlite_exec error %s %d %s\n", __FILE__, __LINE__, errmsg);
        return -1;
    }
    if(send(fd, &row, sizeof(int), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    if(send(fd, &col, sizeof(int), 0) < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    for(i=0; i<col*(row+1); i++)
    {
        send(fd, result[i], 32, 0);
    }
}
