#include "./inc/socket.h"
#include "./inc/op.h"
#include "./inc/menu.h"

void main_loop(int sfd);
void login_operation_administrators(int sfd, package *login_pack);
void login_operation_worker(int sfd, package* login_pack);

int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        printf("error ./Ser ip port\n");
        return -1;
    }
    int sfd;
    int ret;
    struct sockaddr_in sin;
    socklen_t sock_len = sizeof(sin);

    //网络初始化
    sfd = init_socket(argv[1], atoi(argv[2]), &sin);
    if(sfd < 0)
    {
        printf("init socket failed\n");
        return -1;
    }
    printf("socket init success\n");

    ret = connect(sfd, (struct sockaddr*)&sin, sock_len);
    if(ret < 0)
    {
        ERR_MSG("conect");
        return -1;
    }
    printf("connect success\n");

    main_loop(sfd);

    return 0;
}

void main_loop(int sfd)
{
    int op;
    package pack;
    while(1)
    {
        main_menu();
        printf("please input op >> ");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                do_register(sfd);
                break;
            case 2:
                do_login(sfd, &pack);
                trace();
				//printf("pack.id = %d\n", pack.identity);
				if(pack.identity == 'Y')
				{
					trace();
					login_operation_administrators(sfd, &pack);
				}
				else
				{
					trace();
					login_operation_worker(sfd, &pack);
				}
                break;
            case 0:
                do_quit(sfd, &pack);
                return ;
            default:
                printf("input error %s %d\n", __FILE__, __LINE__);
                break;
        }

    }
}

void login_operation_administrators(int sfd, package* login_pack)// 登录的用户
{
    int op;
    while(1)
    {
        login_operation_menu();

        printf("please input your choose >> ");
        scanf("%d", &op);

        switch(op)
        {
            case 1:
                add_worker(sfd);
                break;
            case 2:
                del_worker(sfd);
                break;
            case 3:
                modify_worker(sfd);
                break;
            case 4:
                search_worker(sfd);
                break;
            case 5:
                list_worker(sfd);
                break;
            case 0:
                do_quit(sfd, login_pack);
                break;
        }
        if(op == 0)
        {
            break;
        }
    }

}

void login_operation_worker(int sfd, package* login_pack)// 登录的用户
{
    int op;
    while(1)
    {
        worker_menu();

        printf("please input your choose >> ");
        scanf("%d", &op);

        switch(op)
        {
            case 1:
                search_self(sfd, login_pack);
                break;
            case 2:
                modify_self(sfd, login_pack);
                break;
			case 0:
				do_quit(sfd, login_pack);
				break;
			default:
				break;
        }
        if(op == 0)
        {
            break;
        }
    }

}
