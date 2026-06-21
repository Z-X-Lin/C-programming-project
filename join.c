//包含头文件
#include"join.h"
#include"post.h"
//简化路径名
#define APPLICATIONS_FILE "data/applications.txt"
//创建我的申请
void CreatMyApply(void) {
    clear();
    printf("=====申请搭子=====\n");//小黑窗简单图形
    int postId;
    printf("输入您的ID:");
    scanf("%d",&postId);
    //获取帖子
    post *p=getPostById(postId);
    if(!p){
        printf("帖子不存在！\n");
        pause();
        return;
    }
    //判断是否为自己的帖子
    if(strcmp(p->publisherId,currentUser.ID)==0) {
        printf("不能申请自己发布的帖子！\n");
        pause();
        return;
    }
    //判断状态
    if(p->status!=STATUS_ACTIVE){
        printf("该帖子已不接受申请！\n");
        pause();
        return;
    }
    //不能超出帖主所需容量上限
    if(p->current_number>=p->max_number){
        printf("该帖子人数已满！\n");
        pause();
        return;
    }
    //防止重复
    for(int i=0;i<applicationCount;i++) {
        if(applications[i].postId==postId&&
            strcmp(applications[i].applicantId,currentUser.ID)==0){
            printf("您已申请过该帖子！\n");
            pause();
            return;
        }
    }
    //不能超出申请容量上限
    if(applicationCount>MAX_APPLICATIONS){
        printf("申请已达上限！\n");
        pause();
        return;
    }
    application newApply;
    newApply.postId=postId;
    strcpy(newApply.applicantId,currentUser.ID);
    newApply.approved=0;
    newApply.applyTime=time(NULL);
    printf("请输入联系方式:");
    scanf("%s",newApply.contact);
    printf("请输入申请备注:");
    getchar();
    //stdin：从键盘读取输入
    fgets(newApply.note,sizeof(newApply.note),stdin);
    //返回'\n',在note里的位置并替换为'\0',防止fgets读取'\n'
    newApply.note[strcspn(newApply.note,"\n")]=0;
    applications[applicationCount++]=newApply;
    saveApplications();
    printf("申请成功！等待发布者审核\n");
    pause();
}
//查看我发出的的申请
void ViewMyApply(void){
    clear();
    printf("=====我的申请=====\n");
    int found=0;
    printf("%-8s %-15s %-12s %-20s %-10s\n",
           "序号","帖子标题","联系方式","备注","状态");
    printf("----------------------------------------------------------------\n");
    int displayIdx=0;
    for (int i=0; i<applicationCount;i++){
        if(strcmp(applications[i].applicantId,currentUser.ID)==0&&
            applications[i].approved!=3){
            found=1;
            displayIdx++;
            post *p=getPostById(applications[i].postId);
            char *statusStr="未处理";
            //判断状态
            if(applications[i].approved==1) statusStr="已同意";
            else if(applications[i].approved==2) statusStr="已拒绝";
            else if(applications[i].approved==3) statusStr="已取消";
            printf("%-8d %-15s %-12s %-20s %-10s\n",
                   displayIdx,
                   p?p->title:"未知",
                   applications[i].contact,
                   applications[i].note,
                   statusStr);
        }
    }
    if(!found){
        printf("暂无申请记录\n");
    }
    pause();
}
//处理我得到的申请
void HandleApplications(void){
    clear();
    printf("===== 处理申请 =====\n");
    int myPostIds[MAX_POSTS];
    int myPostCount=0;
    for(int i=0;i<postCount;i++){
        if(strcmp(posts[i].publisherId,currentUser.ID)==0){
            myPostIds[myPostCount++]=posts[i].postId;
        }
    }
    if(myPostCount==0){
        printf("您还没有发布任何帖子，无申请\n");
        pause();
        return;
    }
    int totalApps=0;
    int appList[MAX_APPLICATIONS];
    for(int j=0;j<myPostCount;j++){
        int postId=myPostIds[j];
        post *p=getPostById(postId);
        printf("\n---帖子【%s】(ID:%d)的申请---\n",p?p->title:"未知",postId);
        for(int i=0;i<applicationCount;i++){
            if(applications[i].postId==postId&&applications[i].approved==0){
                totalApps++;
                appList[totalApps-1]=i;
                printf("  序号:%d\n",totalApps);
                printf("  申请人学号:%s\n",applications[i].applicantId);
                printf("  联系方式:%s\n",applications[i].contact);
                printf("  备注:%s\n",applications[i].note);
                printf("  ------------------------\n");
            }
        }
    }
    if(totalApps==0){
        printf("暂无待处理的申请\n");
        pause();
        return;
    }
    printf("\n请输入要处理的申请序号 (0返回): ");
    int applyIdx;
    scanf("%d",&applyIdx);
    if(applyIdx>0&&applyIdx<=totalApps){
        printf("选择处理结果(1:同意 2:拒绝):");
        int approved;
        scanf("%d",&approved);
        if(approved==1||approved==2){
            int realIdx=appList[applyIdx-1];
            applications[realIdx].approved=approved;
            if(approved==1){
                post *p=getPostById(applications[realIdx].postId);
                if(p){
                    p->current_number++;
                    if(p->current_number>=p->max_number){
                        p->status=STATUS_FULL;
                    }
                    savePosts();
                }
            }
            saveApplications();
            printf(approved==1?"已同意该申请！\n":"已拒绝该申请\n");
        }else{
            printf("无效选择！\n");
        }
    }else if(applyIdx!=0){
        printf("无效的申请序号！\n");
    }
    pause();
}
//取消我发出的申请
void CancelApplication(void){
    clear();
    printf("=====取消申请=====\n");
    int displayList[MAX_APPLICATIONS];
    int displayCount=0;//初始数量为零
    for(int i=0;i<applicationCount;i++){
        if(strcmp(applications[i].applicantId,currentUser.ID)==0&&
            applications[i].approved==0){
            displayList[displayCount]=i;
            displayCount++;
        }
    }
    if(displayCount==0){
        printf("暂无可以取消的申请\n");
        pause();
        return;
    }
    printf("%-8s %-15s %-20s %-10s\n",
           "序号","帖子标题","备注","状态");
    printf("------------------------------------------------\n");
    for(int j=0;j<displayCount;j++){
        int i=displayList[j];
        post *p=getPostById(applications[i].postId);
        printf("%-8d %-15s %-20s %-10s\n",
               j+1,
               p?p->title:"未知",
               applications[i].note,
               "未处理");
    }
    printf("\n请输入要取消的申请序号(0返回):");
    int choice;
    scanf("%d",&choice);
    if(choice>0&&choice<=displayCount){
        int idx=displayList[choice-1];
        applications[idx].approved=3;
        saveApplications();
        printf("申请已取消！\n");
    }else if(choice!=0){
        printf("无效的选择！\n");
    }
    pause();
}
//查看我发布的帖子
void ViewMyPublishedPosts(void){
    clear();
    printf("======我发布的帖子======\n");
    int found=0;
    for(int i=0;i<postCount;i++){
        if(strcmp(posts[i].publisherId,currentUser.ID)==0){
            displayPost(&posts[i]);
            found=1;
        }
    }
    if(!found){
        printf("您还没有发布任何帖子\n");
    }
    pause();
}
//加载申请数据
void loadApplications(void){
    //只读形式打开或创建新文件
    FILE *fp=fopen(APPLICATIONS_FILE,"r");
    if(!fp){
        printf("申请文件不存在，将创建新文件\n");
        return;
    }
    //初始申请数为零
    applicationCount=0;
    //提交申请
    while(fscanf(fp,"%d %s %s %s %d %ld",
                  &applications[applicationCount].postId,
                  applications[applicationCount].applicantId,
                  applications[applicationCount].contact,
                  applications[applicationCount].note,
                  &applications[applicationCount].approved,
                  &applications[applicationCount].applyTime)==6){
        applicationCount++;
        //不允许超过最大申请数量
        if(applicationCount>=MAX_APPLICATIONS) break;
    }
    fclose(fp);
    printf("已加载%d条申请记录\n",applicationCount);
}
//保存申请数据
void saveApplications(void){
    //只写形式
    FILE *fp=fopen(APPLICATIONS_FILE,"w");
    if(!fp){
        printf("无法保存申请数据！\n");
        return;
    }
    for(int i=0;i<applicationCount;i++){
        fprintf(fp,"%d %s %s %s %d %ld\n",
                applications[i].postId,
                applications[i].applicantId,
                applications[i].contact,
                applications[i].note,
                applications[i].approved,
                applications[i].applyTime);
    }
    fclose(fp);
}
