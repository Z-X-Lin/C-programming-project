#define __USE_MINGW_ANSI_STDIO 1//强制使用c99/c11标准替代Windows原生输入输出以修复兼容性
#include <stdarg.h>//可变参函数
#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>//时间处理
#include <ctype.h>//字符分类和转换头文件
// 数组最大范围定义（+1 用于存储字符串终止符 '\0'）
#define max_name 21
#define max_password 21
#define max_ID 13
#define max_title 50
#define max_location 100
#define max_contact 30//联系方式
#define max_remark 200//简洁备注
#define ex_max_remark 200//详细备注
// 数组容量定义
#define MAX_USERS 100
#define MAX_POSTS 100
#define MAX_APPLICATIONS 100
// 帖子类型枚举
typedef enum {
    TYPE_CARPOOL = 1,      // 拼车
    TYPE_GROUP_BUY,        // 拼单
    TYPE_STUDY_BUDDY,      // 学习搭子
    TYPE_SPORTS_BUDDY,     // 运动搭子
    TYPE_EAT_BUDDY,        // 吃饭搭子
    TYPE_TRAVEL_BUDDY,     // 旅游搭子
    TYPE_OTHER             // 其他
} posttype;
// 帖子状态枚举
typedef enum {
    STATUS_ACTIVE = 1,     // 进行中
    STATUS_FULL,           // 已满员
    STATUS_EXPIRED,        // 已过期
    STATUS_HIDDEN          // 隐藏
} poststatus;
// 用户结构体
typedef struct {
    char ID[max_ID];
    char name[max_name];
    char password[max_password];
} user;
// 帖子结构体
typedef struct {
    int postId;            // 帖子ID
    char publisherId[max_ID];  // 发布者学号
    posttype type;         // 类型
    char title[max_title]; // 标题
    int max_number;        // 最大人数
    int current_number;    // 当前人数
    char genderlimit;      // 性别要求
    time_t start_time;     // 开始时间,time_t记录
    time_t end_time;       // 截止时间
    char contact[max_contact]; // 联系方式
    double budget;         // 预算
    char location[max_location]; // 地点
    char remark[max_remark];     // 发布者备注
    poststatus status;     // 帖子状态
    time_t publishtime;    // 帖子发布时间
} post;
// 申请结构体
typedef struct {
    int postId;            // 帖子ID
    char applicantId[max_ID];   // 申请人学号
    char contact[max_contact];  // 联系方式
    char note[ex_max_remark];   // 申请者备注
    int approved;          // 是否同意：0.未处理 1.同意 2.拒绝
    time_t applyTime;      // 申请时间
} application;
// 全局变量声明
extern user currentUser;//当前用户
extern user users[MAX_USERS];//所有用户信息
extern int userCount;//记录用户数量
extern post posts[MAX_POSTS];//所有帖子信息
extern int postCount;//记录帖子数量
extern application applications[MAX_APPLICATIONS];//所有申请
extern int applicationCount;//记录申请数量
// 公共函数声明
void clear(void);//清屏
void pause(void);//暂停
#endif