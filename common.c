#include "common.h"
// 全局变量定义
user currentUser = {0};
user users[MAX_USERS];
int userCount = 0;
post posts[MAX_POSTS];
int postCount = 0;
application applications[MAX_APPLICATIONS];
int applicationCount = 0;
// 清屏函数
void clear(void) {
    system("cls");
}
// 暂停函数
void pause(void) {
    printf("\n按回车继续...");
    getchar();
    getchar();
}