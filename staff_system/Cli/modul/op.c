#include "../inc/op.h"
#include "../inc/socket.h"
#include "../inc/menu.h"

static void input_info(package* pack, char* buf)
{
    printf("%s name >> ", buf);
    scanf("%s", pack->name);
    printf("%s passwd >> ", buf);
    scanf("%s", pack->passwd);
    printf("%s phone >> ", buf);
    scanf("%s", pack->phone);
    printf("%s addr >> ", buf);
    scanf("%s", pack->addr);
    printf("%s age >> ", buf);
    scanf("%d", &pack->age);
    printf("%s salary >> ", buf);
    scanf("%f", &pack->salary);
}

static void input_login_info(package* pack)
{
    printf("please input name >>");
    while(getchar()!=10);
    scanf("%s", pack->name);
    printf("please input passwd >>");
    while(getchar()!=10);
    scanf("%s", pack->passwd);
}

int do_register(int sfd)
{
    package pack;
    int ret;
    char buf[32];
    int op = 0;
    pack.cmd = 'R';
    pack.state = 'N';
    while(1)
    {
        register_menu();
        printf("please input your chose >> ");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                pack.identity = 'Y';
                break;
            case 2:
                pack.identity = 'N';
                break;
            case 0:
                return 0;
            default:
                break;
        }
        input_info(&pack, "please input your");

        //发送注册信息
        ret = send(sfd, &pack, PACK_LEN, 0);
        if(ret < 0)
        {
            ERR_MSG("send");
            return -1;
        }

        //接受ACK
        ret = recv(sfd, buf, sizeof(buf), 0);
        if(ret < 0)
        {
            ERR_MSG("recv");
            return -1;
        }
        printf("recv ACK %s\n", buf);
        if(strncmp(buf, "successful", 5) == 0)
        {
            printf("register success\n");
        }
        else
        {
            printf("register failed\n");
        }
    }
    return 0;

}

int do_login(int sfd, package *pack)
{
    int op = 0;
    int ret;
    char buf[32];
    menu_login();

    printf("please input > ");
    while(getchar() != 10);

    scanf("%d", &op);
    //printf("op = %d\n", op);
    switch(op)
    {
    case 0:
        break;
    case 1://管理员登陆
        pack->identity = 'Y';
        break;
    case 2://员工登录
        pack->identity = 'N';
        break;
    default:
        printf("input error\n");
        break;
    }

    input_login_info(pack);
    pack->cmd = 'L';
    pack->state = 'N';

    ret = send(sfd, pack, PACK_LEN, 0);
    if(ret < 0)
    {
        ERR_MSG("send");
        return -1;
    }
    ret = recv(sfd, buf, sizeof(buf), 0);
    if(ret < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    //printf("buf : %s\n", buf);

    if(strncmp(buf, "notin", 5) == 0)
    {
        printf("name is not in info\n");
        return 0;
    }
    else if(strncmp(buf, "nopasswd", 5) == 0)
    {
        printf("passwd error\n");
        return 0;
    }
    else if(strncmp(buf, "nostate", 5) == 0)
    {
        printf("repeat login\n");
        return 0;
    }
    else if(strncmp(buf, "success", 5) == 0)
    {
        printf("login success\n");
        return 0;
    }
    else
    {
        printf("recv error\n");
    }
    return 0;
}

//pack  登陆的信息
int do_quit(int sfd, package* pack)
{

    char buf[32] = "";
    pack->cmd = 'Q';
    strcpy(pack->name, pack->name);
    if(send(sfd, pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }
    
    if(recv(sfd, buf, sizeof(buf), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }

    if(strncmp(buf, "success", 4) == 0)
    {
        printf("quit succcess\n");
    }
    else
    {
        printf("quit failed\n");
    }
    return 0;
}

//pack  登陆的信息
int add_worker(int sfd)
{
    char buf[32] = "";
    package pack;
    input_info(&pack, "input add");
    pack.identity = 'N';
    pack.state = 'N';
    pack.cmd = 'A';
    if(send(sfd, &pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }
    
    if(recv(sfd, buf, sizeof(buf), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }

    if(strncmp(buf, "success", 4) == 0)
    {
        printf("add succcess\n");
    }
    else
    {
        printf("add failed\n");
    }
    return 0;
}

int del_worker(int sfd)
{
    package del_pack;
    char buf[32] = "";
    del_pack.cmd = 'D';
    printf("please input del name > ");
    scanf("%s", del_pack.name);

    if(send(sfd, &del_pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }

    if(recv(sfd, buf, sizeof(buf) ,0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    if(strncmp(buf, "success", 4) == 0)
    {
        printf("del succcess\n");
    }
    else
    {
        printf("del failed\n");
    }
    return 0;

}
int login_operation_quit(int fd)
{
    package pack;
    char buf[32] = "";
    pack.cmd = '0';

    if(send(fd, &pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }
}


int search_worker(int sfd)
{
    package pack;
    char buf[32] = "";
    pack.cmd = 'S';
    int row, col, i;
    char result[32];
    int count = 0;
    printf("please input search name > ");
    scanf("%s", pack.name);

    if(send(sfd, &pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }

    if(recv(sfd, &row, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    //printf("recv row = %d\n", row);
    if(recv(sfd, &col, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    //printf("recv row = %d\n", col);
    for(i=0; i<col*(row+1); i++)
    {
        count++;
        recv(sfd, result, 32, 0);
        printf("%-10s", result);
        if(count%8 == 0)
        {
            printf("\n");
        }
    }
    
    return 0;
}


static void modify_name(package* pack)
{
    printf("please input new name > ");
    bzero(pack->name, sizeof(pack->name));
    scanf("%s", pack->name);
}

static void modify_passwd(package* pack)
{
    printf("please input new passwd > ");
    bzero(pack->passwd, sizeof(pack->passwd));
    scanf("%s", pack->passwd);
}

static void modify_phone(package* pack)
{
    printf("please input new phone > ");
    bzero(pack->phone, sizeof(pack->phone));
    scanf("%s", pack->phone);
}

static void modify_addr(package* pack)
{
    printf("please input new addr > ");
    bzero(pack->addr, sizeof(pack->addr));
    scanf("%s", pack->addr);
}

static void modify_salary(package* pack)
{
    printf("please input new salary > ");
    scanf("%f", &(pack->salary));
}

static void modify_age(package* pack)
{
    printf("please input new age > ");
    scanf("%d", &(pack->age));
}

int modify_worker(int sfd)
{
    package pack;
    pack.cmd = 'M';
    int op = -1;
    char buf[32];
    char name[32];
    printf("input name for modify >> ");
    scanf("%s", pack.name);
    while(1)
    {
        modify_menu();
        printf("please input > ");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                modify_name(&pack);
                break;
            case 2:
                modify_passwd(&pack);
                break;
            case 3:
                modify_phone(&pack);
                break;
            case 4:
                modify_addr(&pack);
                break;
            case 5:
                modify_salary(&pack);
                break;
            case 6:
                modify_age(&pack);
                break;
            case 0:
                return 0;
            default:
                break;
        }
        if(send(sfd, &pack, PACK_LEN, 0) < 0)
        {
            ERR_MSG("send");
            return 0;
        }
 
        printf("mop = %d\n", op);
        if(send(sfd, &op, sizeof(op), 0) < 0)
        {
            ERR_MSG("send");
            return -1;
        }
        // trace();
        if(recv(sfd, buf, sizeof(buf), 0) < 0)
        {
            ERR_MSG("recv");
            return -1;
        }
        // trace();
        if(strncmp(buf, "success", 4) == 0)
        {
            printf("modify succcess\n");
        }
        else
        {
            printf("modify failed\n");
        }
        printf("buf : %s\n", buf);
    }
    return 0;
}

int list_worker(int sfd)
{
    package pack;
    char buf[32] = "";
    pack.cmd = 'H';
    int row, col, i;
    char result[32];
    int count = 0;


    if(send(sfd, &pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }

    if(recv(sfd, &row, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    printf("recv row = %d\n", row);
    if(recv(sfd, &col, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    printf("recv row = %d\n", col);
    for(i=0; i<col*(row+1); i++)
    {
        count++;
        recv(sfd, result, 32, 0);
        printf("%-10s", result);
        if(count%8 == 0)
        {
            printf("\n");
        }
    }
    
    return 0;
}


int modify_self(int sfd, package* login_pack)
{
    package pack;
    pack.cmd = 'M';
    int op = -1;
    char buf[32];

	strcpy(pack.name, login_pack->name);

    while(1)
    {
        modify_menu();
        printf("please input > ");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                break;
                modify_name(&pack);
                break;
            case 2:
                modify_passwd(&pack);
                break;
            case 3:
                modify_phone(&pack);
                break;
            case 4:
                modify_addr(&pack);
                break;
            case 5:
                modify_salary(&pack);
                break;
            case 6:
                modify_age(&pack);
                break;
            case 0:
                return 0;
            default:
                break;
        }
        if(send(sfd, &pack, PACK_LEN, 0) < 0)
        {
            ERR_MSG("send");
            return 0;
        }
 
        printf("mop = %d\n", op);
        if(send(sfd, &op, sizeof(op), 0) < 0)
        {
            ERR_MSG("send");
            return -1;
        }
        // trace();
        if(recv(sfd, buf, sizeof(buf), 0) < 0)
        {
            ERR_MSG("recv");
            return -1;
        }
        // trace();
        if(strncmp(buf, "success", 4) == 0)
        {
            printf("modify succcess\n");
        }
        else
        {
            printf("modify failed\n");
        }
        printf("buf : %s\n", buf);
    }
    return 0;
}


int search_self(int sfd, package* login_pack)
{
    package pack;
    char buf[32] = "";
    pack.cmd = 'S';
    int row, col, i;
    char result[32];
    int count = 0;
	
	strcpy(pack.name, login_pack->name);

    if(send(sfd, &pack, PACK_LEN, 0) < 0)
    {
        ERR_MSG("send");
        return 0;
    }

    if(recv(sfd, &row, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    printf("recv row = %d\n", row);
    if(recv(sfd, &col, sizeof(int), 0) < 0)
    {
        ERR_MSG("recv");
        return -1;
    }
    printf("recv row = %d\n", col);
    for(i=0; i<col*(row+1); i++)
    {
        count++;
        recv(sfd, result, 32, 0);
        printf("%-10s", result);
        if(count%8 == 0)
        {
            printf("\n");
        }
    }
    
    return 0;
}