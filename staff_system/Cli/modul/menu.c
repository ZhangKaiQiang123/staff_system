#include "../inc/menu.h"
#include "../inc/socket.h"

void main_menu(void)
{
    printf("\t\t****  1:register  ****\n\n");
    printf("\t\t****  2:login     ****\n\n");
    printf("\t\t****  0:quit      ****\n\n");
}

void register_menu(void)
{
    printf("\t\t****  1:admi      ****\n\n");
    printf("\t\t****  2:worker    ****\n\n");
    printf("\t\t****  0:quit      ****\n\n");
}

void menu_login(void)
{
    printf("\t\t****  1:admi      ****\n\n");
    printf("\t\t****  2:worker    ****\n\n");
    printf("\t\t****  0:quit      ****\n\n");
}

void login_operation_menu(void)
{
    printf("\t\t****  1:add      ****\n\n");
    printf("\t\t****  2:del      ****\n\n");
    printf("\t\t****  3:mod      ****\n\n"); 
    printf("\t\t****  4:sea      ****\n\n");
    printf("\t\t****  5:lis      ****\n\n");
    printf("\t\t****  0:mod      ****\n\n"); 
}

void modify_menu(void)
{
	// printf("\t\t****  1:modify name    ****\n\n");
	printf("\t\t****  2:modify passwd  ****\n\n");
	printf("\t\t****  3:modify phone   ****\n\n");	
	printf("\t\t****  4:modify addr    ****\n\n");
	printf("\t\t****  5:modify salary  ****\n\n");	
	printf("\t\t****  6:modify age     ****\n\n");
	printf("\t\t****  0:exit           ****\n");	
}

void worker_menu(void)
{
    printf("\t\t****  1:show      ****\n\n");
    printf("\t\t****  2:modify    ****\n\n");
    printf("\t\t****  0:exit      ****\n\n"); 
}