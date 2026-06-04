#include "common.h"
#include "user.h"
#include "post.h"
#include "join.h"

void userMenu(void) {
    char choice;
    while (1) {
        clear();
        printf("===== 校园搭子系统 =====\n");
        printf("当前登录：%s (%s)\n", currentUser.name, currentUser.ID);
        printf("1. 发布搭子\n");
        printf("2. 查看所有搭子\n");
        printf("3. 搜索搭子\n");
        printf("4. 分类浏览\n");
        printf("5. 申请搭子\n");
        printf("6. 我的申请\n");
        printf("7. 取消申请\n");
        printf("8. 处理申请\n");
        printf("9. 批量处理\n");
        printf("0. 我发布的搭子\n");
        printf("------------------\n");
        printf("a. 个人信息\n");
        printf("b. 修改密码\n");
        printf("c. 注销账号\n");
        printf("d. 退出登录\n");
        printf("请选择：");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                postAdd();
                break;
            case '2':
                postListAll();
                break;
            case '3':
                searchPost();
                break;
            case '4':
                {
                    clear();
                    printf("===== 分类浏览 =====\n");
                    printf("1. 拼车\n");
                    printf("2. 拼单\n");
                    printf("3. 学习搭子\n");
                    printf("4. 运动搭子\n");
                    printf("5. 吃饭搭子\n");
                    printf("6. 旅游搭子\n");
                    printf("7. 其他\n");
                    printf("请选择类型：");
                    int type;
                    scanf("%d", &type);
                    if (type >= 1 && type <= 7) {
                        postListByType(type);
                    } else {
                        printf("无效选择！\n");
                        pause();
                    }
                }
                break;
            case '5':
                MyApply();
                break;
            case '6':
                MyList();
                break;
            case '7':
                CancelApplication();
                break;
            case '8':
                HandleApplications();
                break;
            case '9':
                BatchHandle();
                break;
            case '0':
                ViewMyPublishedPosts();
                break;
            case 'a':
            case 'A':
                userInfo();
                break;
            case 'b':
            case 'B':
                userModifyPassword();
                break;
            case 'c':
            case 'C':
                userDelete();
                if (currentUser.ID[0] == '\0') {
                    return;
                }
                break;
            case 'd':
            case 'D':
                memset(&currentUser, 0, sizeof(user));
                printf("已退出登录\n");
                pause();
                return;
            default:
                printf("输入错误！\n");
                pause();
        }
    }
}

int main(void) {
    int choice;

    loadUsers();
    loadPosts();
    loadApplications();

    while (1) {
        clear();
        printf("===== 校园搭子平台 =====\n");
        printf("1. 注册\n");
        printf("2. 登录\n");
        printf("0. 退出\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                userRegister();
                break;
            case 2:
                if (userLogin()) {
                    userMenu();
                }
                break;
            case 0:
                clear();
                printf("程序已退出\n");
                return 0;
            default:
                printf("输入错误！\n");
                pause();
        }
    }
}