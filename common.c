#include "common.h"
// 全局变量定义及初始化
//当前用户
user currentUser={0};
//用户数组
user users[MAX_USERS];
//初始化当前用户数量为0
int userCount=0;
//帖子结构体
post posts[MAX_POSTS];
//初始化当前帖子数量为0
int postCount=0;
//帖子ID,序号每次自动加一
int nextPostId=1;
//申请结构体
application applications[MAX_APPLICATIONS];
//初始化当前申请数量为0
int applicationCount=0;
//清屏函数
void clear(void){
    system("cls");
}
//暂停函数
void pause(void){
    printf("\n按回车继续...");
    getchar();
    getchar();
}