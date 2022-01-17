#include <pthread.h>
#include "./inc/socket.h"
#include "./inc/sqlite.h"
#include "./inc/op.h"

void operation(int newfd, package* pack, sqlite3* db);

#if 1
int do_use_fd(int newfd, int epfd, sqlite3* db);
int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        printf("error ./Ser ip port\n");
        return -1;
    }

    int sfd, newfd, epfd, nfds;
    struct sockaddr_in sin, cin;
    socklen_t sock_len = sizeof(sin);
    sqlite3 * db;
    int ret, i;
    struct epoll_event event;
    struct epoll_event *pevents;
    pevents = (struct epoll_event *)malloc(MAX_EVENTS * sizeof(struct epoll_event));
    if(pevents == NULL)
    {
        ERR_MSG("malloc");
        return -1;
    }

    //网络初始化
    sfd = init_socket(argv[1], atoi(argv[2]), &sin);
    if(sfd < 0)
    {
        printf("init socket failed\n");
        return -1;
    }
    printf("socket init success\n");

    //数据库初始化
    ret = init_sqlite(&db);
    if(ret < 0)
    {
        printf("init sqlite failed\n");
        return -1;
    }
    printf("init sqlite success\n");

    //创建epoll
    epfd = epoll_create(10);
    if(epfd < 0)
    {
        ERR_MSG("epoll");
        return -1;
    }

    //将sfd加入epoll队列
    event.events = EPOLLIN;
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    if(ret == -1)
    {
        ERR_MSG("epoll_ctl");
        return -1;
    }

    while(1)
    {
        //epoll_wait
        nfds = epoll_wait(epfd, pevents, MAX_EVENTS, -1);
        if(nfds < 0)
        {
            ERR_MSG("epool_wait");
            return -1;
        }
        
        for(i=0; i<nfds; i++)
        {
            if(!(pevents[i].events & EPOLLIN))
            {
                continue;
            }
            //判断是否为连接事件
            if(pevents[i].data.fd == sfd)
            {
                newfd = accept(sfd, (struct sockaddr*)&cin, &sock_len);
                if(newfd < 0)
                {
                    ERR_MSG("accept");
                    return -1;
                }
                //添加到epoll队列
                event.events = EPOLLIN;
                event.data.fd = newfd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &event);
                if(ret < 0)
                {
                    ERR_MSG("epoll_ctl");
                    return -1;
                }
                printf("Connect %d\n", newfd);

            }
            else
            {
                //客户端交互事件
                do_use_fd(pevents[i].data.fd, epfd, db);
            }
        }
    }
    
    return 0;
}


int do_use_fd(int newfd, int epfd, sqlite3* db)
{
    int ret, n;
    char buf[128];
    package pack;
    struct epoll_event event;

    //读到 -1 出错

    bzero(buf, sizeof(buf));
    n = recv(newfd, &pack, PACK_LEN, 0);
    if(n == 0)
    {
        //读到0 客户端关闭连接
        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, newfd, NULL);
        if(ret < 0)
        {
            ERR_MSG("epoll_ctl");
            return -1;
        }
        do_quit(newfd, &pack, db);
        close(newfd);
    }
    else if(n < 0)
    {
        ERR_MSG("read");
        return -1;
    }
    else
    {
        printf("pack1\n");
        operation(newfd, &pack, db);
    }

    return 0;
}
#else

void* call_back(void *arg); 

typedef struct
{
    int newfd;
    struct sockaddr_in cin;
    sqlite3* db;
}Msg; 

int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        printf("error ./Ser ip port\n");
        return -1;
    }

    int sfd, newfd;
    struct sockaddr_in sin, cin;
    socklen_t sock_len = sizeof(sin);
    sqlite3 * db;
    int ret, i;
    pthread_t tid;  
    Msg msg; 

    //网络初始化
    sfd = init_socket(argv[1], atoi(argv[2]), &sin);
    if(sfd < 0)
    {
        printf("init socket failed\n");
        return -1;
    }
    printf("socket init success\n");

    //数据库初始化
    ret = init_sqlite(&db);
    if(ret < 0)
    {
        printf("init sqlite failed\n");
        return -1;
    }
    printf("init sqlite success\n");

    while(1)
    {

        newfd = accept(sfd, (struct sockaddr *)&cin, &sock_len);
        if(newfd < 0)
        {
            ERR_MSG("accept");
            return -1; 
        }
        printf("client connect fd = %d\n", newfd);
                                                                                                                                  
        msg.cin = cin;
        msg.db = db; 
        msg.newfd = newfd;

        ret = pthread_create(&tid, NULL, call_back, (void*)&msg);
        if(ret < 0)
        {
            ERR_MSG("pthread_Create");
            return -1; 
        }
 
    }
    return 0;
}

void *call_back(void *arg)
{
    pthread_detach(pthread_self());

    Msg *msg = (Msg*)arg;
    struct sockaddr_in cin = msg->cin;
    int newfd = msg->newfd;
    sqlite3* db = msg->db;
    package pack;
    package newpack;
    package login_pack;
    int ret = 0;
    while(1)                                                                                                                      
    {
        ret = recv(newfd, &pack, sizeof(pack), 0);
        if(ret < 0)
        {
            ERR_MSG("recv");
            return NULL;
        }
        else if(ret == 0)
        {
            printf("cli : %d  close\n", newfd);
            do_quit(newfd, &pack, db);
            close(newfd);
            return NULL;
        }
        else
        {
            printf("pack1\n");
            operation(newfd, &pack, db);
        }
    }
}
#endif

int login_operation(int newfd, package* pack, sqlite3* db);
void operation(int newfd, package* pack, sqlite3* db)
{
    switch(pack->cmd)
    {
        case 'R':
            do_register(newfd, pack, db);
            break;
        case 'L':
            do_login(newfd, pack, db);
            login_operation(newfd, pack, db);
            break;
        case 'Q':
            do_quit(newfd, pack, db);
            break;
    }
}

int login_operation(int newfd, package* pack, sqlite3* db)
{
    package newpack;
    int ret;
    while(1)
    {
        ret = recv(newfd, &newpack, PACK_LEN, 0);
        if(ret < 0)
        {
            ERR_MSG("recv");
            return -1;
        }
        if(ret == 0)
        {
            return 0;
        }

        switch(newpack.cmd)
        {
            case 'A':
                add_worker(newfd, &newpack, db);
                break;
            case 'D':
                del_worker(newfd, &newpack, db);
                break;
            case 'S':
                search_worker(newfd, &newpack, db);
                break;
            case 'M':
                modify_worker(newfd, &newpack, db);
                break;
            case 'Q':
                do_quit(newfd, pack, db);
                break;
            case 'H':
				list_worker(newfd, pack, db);
				break;
        }
        if(newpack.cmd == 'Q')
        {
            return 0;
        }
    }
    return 0;
}
