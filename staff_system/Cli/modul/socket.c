#include "../inc/socket.h"

int init_socket(const char* ip, const short port, struct sockaddr_in* sin)
{
    int sfd = 0;
    int result = 1;
    int ret = 0;

    //创建流式套接字
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
    {
        ERR_MSG("socket");
        return -1;
    }

    //允许端口快速重用
    ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &result, sizeof(result));
    if(ret < 0)
    {
        ERR_MSG("setsockopt");
        return -1;
    }

    //填充地址信息结构体
    sin->sin_family = AF_INET;
    sin->sin_port = htons(port);
    sin->sin_addr.s_addr = inet_addr(ip);

    return sfd;
}