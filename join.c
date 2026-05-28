#include "join.h"
#include "post.h"//包含头文件
#define APPLICATIONS_FILE "data/applications.txt"//简化路径名
//加载
void loadApplications(void) {
    //打开或创建新文件
    FILE *fp = fopen(APPLICATIONS_FILE, "r");//只读
    if (!fp) {
        printf("申请文件不存在，将创建新文件\n");
        return;
    }
    applicationCount = 0;//初始申请数为零
    //提交申请
    while (fscanf(fp, "%d %s %s %s %d %ld",
                  &applications[applicationCount].postId,
                  applications[applicationCount].applicantId,
                  applications[applicationCount].contact,
                  applications[applicationCount].note,
                  &applications[applicationCount].approved,
                  &applications[applicationCount].applyTime) == 6) {
        applicationCount++;
        if (applicationCount >= MAX_APPLICATIONS) break;
    }
    fclose(fp);
    printf("已加载 %d 条申请记录\n", applicationCount);
}
//保存申请数据
void saveApplications(void) {
    FILE *fp = fopen(APPLICATIONS_FILE, "w");
    if (!fp) {
        printf("无法保存申请数据！\n");
        return;
    }
    for (int i = 0; i < applicationCount; i++) {
        fprintf(fp, "%d %s %s %s %d %ld\n",
                applications[i].postId,
                applications[i].applicantId,
                applications[i].contact,
                applications[i].note,
                applications[i].approved,
                applications[i].applyTime);
    }
    fclose(fp);
}
//创建我的申请
void MyApply(void) {
    clear();//清屏
    printf("===== 申请搭子 =====\n");
    int postId;
    printf("输入搭子ID：");
    scanf("%d", &postId);
    post *p = getPostById(postId);//获取
    if (!p) {
        printf("搭子不存在！\n");
        pause();
        return;
    }
    //判断是否为自己的
    if (strcmp(p->publisherId, currentUser.ID) == 0) {
        printf("不能申请自己发布的搭子！\n");
        pause();
        return;
    }
    //判断状态
    if (p->status != STATUS_ACTIVE) {
        printf("该搭子已不接受申请！\n");
        pause();
        return;
    }
    if (p->current_number >= p->max_number) {
        printf("该搭子人数已满！\n");
        pause();
        return;
    }
    //防止重复
    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].postId == postId &&
            strcmp(applications[i].applicantId, currentUser.ID) == 0) {
            printf("您已申请过该搭子！\n");
            pause();
            return;
        }
    }
    if (applicationCount >= MAX_APPLICATIONS) {
        printf("申请已达上限！\n");
        pause();
        return;
    }
//创建新的申请
    application newApp;
    newApp.postId = postId;
    strcpy(newApp.applicantId, currentUser.ID);
    newApp.approved = 0;
    newApp.applyTime = time(NULL);
    printf("请输入联系方式: ");
    scanf("%s", newApp.contact);
    printf("请输入申请备注: ");
    getchar();
    fgets(newApp.note, sizeof(newApp.note), stdin);//stdin：从键盘读取输入
    newApp.note[strcspn(newApp.note, "\n")] = 0;//返回'\n',在note里的位置并替换为'\0'
                                                //防止fgets读取'\n'
    applications[applicationCount++] = newApp;
    saveApplications();
    printf("申请成功！等待发布者审核\n");
    pause();
}
//查看我的申请
void MyList(void) {
    clear();
    printf("===== 我的申请 =====\n");
    int found = 0;
    printf("%-8s %-15s %-12s %-20s %-10s\n",
           "序号", "帖子标题", "联系方式", "备注", "状态");
    printf("----------------------------------------------------------------\n");
    int displayIdx = 0;
    for (int i = 0; i < applicationCount; i++) {
        if (strcmp(applications[i].applicantId, currentUser.ID) == 0 &&
            applications[i].approved != 3) {
            found = 1;
            displayIdx++;
            post *p = getPostById(applications[i].postId);
            char *statusStr = "未处理";
            //判断状态
            if (applications[i].approved == 1) statusStr = "已同意";
            else if (applications[i].approved == 2) statusStr = "已拒绝";
            else if (applications[i].approved == 3) statusStr = "已取消";
            printf("%-8d %-15s %-12s %-20s %-10s\n",
                   displayIdx,
                   p ? p->title : "未知",
                   applications[i].contact,
                   applications[i].note,
                   statusStr);
        }
    }
    if (!found) {
        printf("暂无申请记录\n");
    }
    pause();
}
//取消我发出的申请
void CancelApplication(void) {
    clear();
    printf("===== 取消申请 =====\n");
    int displayList[MAX_APPLICATIONS];//展示的最大数量为MAX_APPLICATIONS
    int displayCount = 0;//初始数量为零
    for (int i = 0; i < applicationCount; i++) {
        if (strcmp(applications[i].applicantId, currentUser.ID) == 0 &&
            applications[i].approved == 0) {
            displayList[displayCount] = i;
            displayCount++;
        }
    }
    if (displayCount == 0) {
        printf("暂无可以取消的申请\n");
        pause();
        return;
    }
    printf("%-8s %-15s %-20s %-10s\n",
           "序号", "帖子标题", "备注", "状态");
    printf("------------------------------------------------\n");
    for (int j = 0; j < displayCount; j++) {
        int i = displayList[j];
        post *p = getPostById(applications[i].postId);
        printf("%-8d %-15s %-20s %-10s\n",
               j + 1,
               p ? p->title : "未知",
               applications[i].note,
               "未处理");
    }
    printf("\n请输入要取消的申请序号 (0返回): ");
    int choice;
    scanf("%d", &choice);
    if (choice > 0 && choice <= displayCount) {
        int idx = displayList[choice - 1];
        applications[idx].approved = 3;
        saveApplications();
        printf("申请已取消！\n");
    } else if (choice != 0) {
        printf("无效的选择！\n");
    }
    pause();
}
//查看我发布的搭子
void ViewMyPublishedPosts(void) {
    clear();
    printf("===== 我发布的搭子 =====\n");
    int found = 0;
    printf("%-5s %-10s %-15s %-8s %-10s\n",
           "ID", "类型", "标题", "人数", "状态");
    printf("------------------------------------------------\n");
    for (int i = 0; i < postCount; i++) {
        if (strcmp(posts[i].publisherId, currentUser.ID) == 0) {
            found = 1;
            char *typeName = getTypeName(posts[i].type);
            char statusStr[20];
            switch (posts[i].status) {
                case STATUS_ACTIVE: strcpy(statusStr, "进行中"); break;
                case STATUS_FULL: strcpy(statusStr, "已满员"); break;
                case STATUS_EXPIRED: strcpy(statusStr, "已过期"); break;
                default: strcpy(statusStr, "未知"); break;
            }
            printf("%-5d %-10s %-15s %d/%d    %-10s\n",
                   posts[i].postId,
                   typeName,
                   posts[i].title,
                   posts[i].current_number,
                   posts[i].max_number,
                   statusStr);
        }
    }
    if (!found) {
        printf("您还没有发布任何搭子\n");
    }
    pause();
}
//处理我得到的申请
void HandleApplications(void) {
    clear();
    printf("===== 处理申请 =====\n");
    int myPostIds[MAX_POSTS];
    int myPostCount = 0;
    for (int i = 0; i < postCount; i++) {
        if (strcmp(posts[i].publisherId, currentUser.ID) == 0) {
            myPostIds[myPostCount++] = posts[i].postId;
        }
    }
    if (myPostCount == 0) {
        printf("您还没有发布任何帖子，无申请\n");
        pause();
        return;
    }
    int totalApps = 0;
    int appList[MAX_APPLICATIONS];
    for (int j = 0; j < myPostCount; j++) {
        int postId = myPostIds[j];
        post *p = getPostById(postId);
        printf("\n--- 帖子【%s】(ID:%d)的申请 ---\n", p ? p->title : "未知", postId);
        for (int i = 0; i < applicationCount; i++) {
            if (applications[i].postId == postId && applications[i].approved == 0) {
                totalApps++;
                appList[totalApps - 1] = i;
                printf("  序号: %d\n", totalApps);
                printf("  申请人学号: %s\n", applications[i].applicantId);
                printf("  联系方式: %s\n", applications[i].contact);
                printf("  备注: %s\n", applications[i].note);
                printf("  ------------------------\n");
            }
        }
    }
    if (totalApps == 0) {
        printf("暂无待处理的申请\n");
        pause();
        return;
    }
    printf("\n请输入要处理的申请序号 (0返回): ");
    int applyIdx;
    scanf("%d", &applyIdx);
    if (applyIdx > 0 && applyIdx <= totalApps) {
        printf("选择处理结果 (1:同意 2:拒绝): ");
        int approved;
        scanf("%d", &approved);
        if (approved == 1 || approved == 2) {
            int realIdx = appList[applyIdx - 1];
            applications[realIdx].approved = approved;
            if (approved == 1) {
                post *p = getPostById(applications[realIdx].postId);
                if (p) {
                    p->current_number++;
                    if (p->current_number >= p->max_number) {
                        p->status = STATUS_FULL;
                    }
                    savePosts();
                }
            }
            saveApplications();
            printf(approved == 1 ? "已同意该申请！\n" : "已拒绝该申请！\n");
        } else {
            printf("无效选择！\n");
        }
    } else if (applyIdx != 0) {
        printf("无效的申请序号！\n");
    }
    pause();
}
//批量处理申请
void BatchHandle(void) {
    clear();
    printf("===== 批量处理申请 =====\n");
    int myPostIds[MAX_POSTS];
    int myPostCount = 0;
    for (int i = 0; i < postCount; i++) {
        if (strcmp(posts[i].publisherId, currentUser.ID) == 0) {
            myPostIds[myPostCount++] = posts[i].postId;
        }
    }
    if (myPostCount == 0) {
        printf("您还没有发布任何帖子，无申请\n");
        pause();
        return;
    }
    int appList[MAX_APPLICATIONS];
    int appCount = 0;
    for (int j = 0; j < myPostCount; j++) {
        int postId = myPostIds[j];
        for (int i = 0; i < applicationCount; i++) {
            if (applications[i].postId == postId && applications[i].approved == 0) {
                appList[appCount++] = i;
            }
        }
    }
    if (appCount == 0) {
        printf("暂无待处理的申请\n");
        pause();
        return;
    }
    printf("待处理申请数: %d\n", appCount);
    printf("1. 全部同意\n");
    printf("2. 全部拒绝\n");
    printf("0. 返回\n");
    printf("请选择: ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1 || choice == 2) {
        int approved = choice;
        for (int k = 0; k < appCount; k++) {
            int idx = appList[k];
            applications[idx].approved = approved;
            //更新数据
            if (approved == 1) {
                post *p = getPostById(applications[idx].postId);
                if (p) {
                    p->current_number++;
                    if (p->current_number >= p->max_number) {
                        p->status = STATUS_FULL;
                    }
                }
            }
        }
        if (approved == 1) {
            savePosts();
        }
        saveApplications();
        printf(approved == 1 ? "已全部同意！\n" : "已全部拒绝！\n");
    } else if (choice != 0) {
        printf("无效选择！\n");
    }
    pause();
}