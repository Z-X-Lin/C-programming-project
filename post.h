#ifndef POST_H
#define POST_H

#include "common.h"

extern post posts[MAX_POSTS];
extern int postCount;
extern int nextPostId;

void postAdd(void);//发布帖子函数
void postListAll(void);//显示所有帖子函数
void postListByType(int type);//根据类型显示帖子函数
void savePosts(void);//保存帖子函数
void loadPosts(void);// 加载帖子函数
post* getPostById(int id);//根据ID获取帖子的函数
char* getTypeName(int type);//根据类型获取类型名称的函数
char* getStatusName(poststatus status);//根据状态获取状态名称的函数
void updatePost(post *p);//更新帖子的函数
void updatePostStatus(post *p);//更新帖子状态的函数ssss
void displayPost(post *p);//显示单个帖子详情的函数
void sortPosts(void);//排序帖子函数
void searchPost(void);//搜索帖子函数
void ViewMyPublishedPosts(void);//查看我发布的搭子函数

#endif



