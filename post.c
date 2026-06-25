#include "post.h"

char* getTypeName(int type){ 
    switch(type){ 
        case 1:return "拼车"; 
        case 2:return "拼单"; 
        case 3:return "学习搭子"; 
        case 4:return "运动搭子"; 
        case 5:return "吃饭搭子"; 
        case 6:return "旅游搭子"; 
        case 7:return "其他"; 
        default:return "Unknown"; 
    } 
}

void sortPosts(void){
    for(int i = 0; i < postCount - 1; i++){
        for(int j = 0; j < postCount - i - 1; j++){
            if(posts[j].publishtime < posts[j + 1].publishtime){
                post temp = posts[j];
                posts[j] = posts[j + 1];
                posts[j + 1] = temp;
            }
        }
    }
}

void loadPosts(void){
    postCount = 0;
    nextPostId = 1;

    FILE *fp = fopen("data/posts.txt","r");
    
    if(fp == NULL){
        printf("File Open Failed!\n");
        return;
    }

    printf("File Open Success!\n");
    
    while(fscanf(fp,"%d|%[^|]|%[^|]|%d|%d|%d|%c|%lld|%lld|%d|%[^|]|%[^|]|%lf|%[^|]|%lld\n", //%[^|\n]表示读取直到遇到|或换行的字符串停止
           &posts[postCount].postId,
           posts[postCount].publisherId,
           posts[postCount].title,
           &posts[postCount].type,
           &posts[postCount].current_number,
           &posts[postCount].max_number,
           &posts[postCount].genderlimit,
           &posts[postCount].start_time,
           &posts[postCount].end_time,  
           &posts[postCount].status, 
           posts[postCount].location,
           posts[postCount].contact,
           &posts[postCount].budget,
           posts[postCount].remark,
           &posts[postCount].publishtime) == 15)
    {
        printf("Load One Post\n");
        
        if(posts[postCount].current_number >= posts[postCount].max_number){
            posts[postCount].status = STATUS_FULL;
        }else{
            posts[postCount].status = STATUS_ACTIVE;
        }
        
        postCount++;
        
        if(posts[postCount - 1].postId >= nextPostId){
            nextPostId = posts[postCount - 1].postId + 1;
        }
    }
    
    for(int i = 0; i < postCount; i++){
        printf("帖子编号:%d\n", posts[i].postId);
        printf("标题:%s\n", posts[i].title);
        printf("类型:%s\n", getTypeName(posts[i].type));
        printf("成员: %d/%d\n", posts[i].current_number, posts[i].max_number);
        printf("位置:%s\n", posts[i].location);
        printf("联系方式:%s\n", posts[i].contact);
        printf("预算:%.2f\n", posts[i].budget);
        printf("备注:%s\n", posts[i].remark);
    }

     fclose(fp);
}

void postAdd(void){
    post newPost;
    newPost.publishtime = time(NULL);//记录当前时间为发布时间
    newPost.current_number =1;//初始当前人数为1

    strcpy(newPost.publisherId, currentUser.ID);//把当前用户ID作为发布者ID
    
    newPost.postId = nextPostId++;//把当前编号给新帖子
    
    printf("输入标题: ");
    scanf("%s", newPost.title);

    printf("请选择搭子类型：\n");
    printf("1. 拼车\n");
    printf("2. 拼单\n");
    printf("3. 学习搭子\n");
    printf("4. 运动搭子\n");
    printf("5. 吃饭搭子\n");
    printf("6. 旅游搭子\n");
    printf("7. 其他\n");
    printf("输入类型(1-7): ");
    scanf("%d", &newPost.type);

    printf("输入最大人数: ");
    scanf("%d", &newPost.max_number);

    printf("性别限制 (M/F/A): ");
    scanf(" %c", &newPost.genderlimit);

    printf("输入持续时间(天):");
    int days;
    scanf("%d", &days);

    newPost.start_time = time(NULL);
    newPost.end_time = newPost.start_time + days * 24 * 3600;//截止时间为开始时间加上持续天数

    if(newPost.current_number >= newPost.max_number){
        newPost.status = STATUS_FULL;
    }else{
        newPost.status = STATUS_ACTIVE;
    }

    printf("输入位置:");
    scanf("%s", newPost.location);

    printf("输入联系方式:");
    scanf("%s", newPost.contact);

    printf("输入预算:");
    scanf("%lf", &newPost.budget);

    printf("输入备注:");
    scanf("%s", newPost.remark);

    printf("帖子编号:%d\n", newPost.postId);
    printf("标题: %s\n", newPost.title);
    printf("类型: %s\n", getTypeName(newPost.type));
    printf("成员: %d/%d\n", newPost.current_number, newPost.max_number);
    printf("位置: %s\n", newPost.location);
    printf("联系方式: %s\n", newPost.contact);
    printf("预算: %.2f\n", newPost.budget);
    printf("备注: %s\n", newPost.remark);
    posts[postCount] = newPost;//把新帖子放到数组里
    postCount++;

    savePosts();//保存到文件

    printf("Post added successfully!\n");
    printf("Current Post Count: %d\n", postCount);
}

void savePosts(void){
    FILE *fp = fopen("data/posts.txt","w");
    
    if(fp == NULL){
        printf("File Open Failed\n");
        return;
    }

    printf("post.txt opened successfully!\n");

    for(int i = 0;i < postCount;i++){
        fprintf(fp,"%d|%s|%s|%d|%d|%d|%c|%lld|%lld|%d|%s|%s|%.2lf|%s|%lld\n",
            posts[i].postId,
            posts[i].publisherId,
            posts[i].title,
            posts[i].type,
            posts[i].current_number,
            posts[i].max_number,
            posts[i].genderlimit,
            (long long)posts[i].start_time,
            (long long)posts[i].end_time,
            posts[i].status,
            posts[i].location,
            posts[i].contact,
            posts[i].budget,
            posts[i].remark,
            (long long)posts[i].publishtime);
    }
    
    fclose(fp);
}

void displayPost(post *p){
    char timeStr[100];
    strftime(
        timeStr, sizeof(timeStr), 
        "%Y-%m-%d %H:%M:%S", 
        localtime(&p->publishtime)
    );

    char startStr[100];
    char endStr[100];
    strftime(
        startStr, sizeof(startStr),
        "%Y-%m-%d %H:%M:%S",
        localtime(&p->start_time)
    );
    strftime(
        endStr, sizeof(endStr),
        "%Y-%m-%d %H:%M:%S",
        localtime(&p->end_time)
    );

    printf("\n");
    printf("帖子编号:%d\n", p->postId);
    printf("发布者ID:%s\n", p->publisherId);
    printf("标题:%s\n", p->title);
    printf("类型:%s\n", getTypeName(p->type));
    printf("当前人数:%d/%d\n", p->current_number, p->max_number);
    printf("性别限制:%c\n", p->genderlimit);
    printf("状态:%s\n", getStatusName(p->status));
    printf("开始时间:%s\n", startStr);
    printf("结束时间:%s\n", endStr);
    printf("位置:%s\n", p->location);
    printf("联系方式:%s\n", p->contact);
    printf("预算:%.2f\n", p->budget);
    printf("备注:%s\n", p->remark);
    printf("发布时间:%s\n", timeStr);
    printf("------------------------------\n");
    
}

void postListAll(void){
    sortPosts();
    for(int i = 0;i < postCount;i++){
        displayPost(&posts[i]);
    }

    pause();
}

post* getPostById(int id){
    for(int i = 0; i < postCount; i++){
        if(posts[i].postId == id){
            return &posts[i];//返回指向该帖子的指针
        }
    }
    return NULL;//没找到Id返回NULL
}

void updatePostStatus(post *p){
    if(p == NULL){
        return;
    }

    if(p -> current_number >= p -> max_number){
        p -> status = STATUS_FULL;
    }else{
        p -> status = STATUS_ACTIVE;
    }//更新状态
}

void updatePost(post *p){
    if(p == NULL){
        return;
    }

    p -> current_number++;//增加当前人数

    updatePostStatus(p);//更新状态

    savePosts();
}

void searchPost(void){
    int id;
    
    printf("Input Post ID:");
    scanf("%d", &id);
    post *p = getPostById(id);

    if(p == NULL){
        printf("Post not found!\n");
        return;
    }

    displayPost(p);

    pause();
}

void postListByType(int type){
    sortPosts();
    
    int found = 0;

    printf("\n==== %s ====\n", getTypeName(type));
    
    for(int i = 0; i < postCount; i++){
        if(posts[i].type == type){
            displayPost(&posts[i]);
            found = 1;
        }
    }
    if(!found){
        printf("No posts of this type found!\n");
    }

    pause();
}

char* getStatusName(poststatus status){
    switch(status){
        case STATUS_ACTIVE:
            return "Active";
        case STATUS_FULL:
            return "Full";
        case STATUS_EXPIRED:
            return "Expired";
        case STATUS_HIDDEN:
            return "Hidden";
        default:
            return "Unknown";
    }
}

void ViewMyPublishedPosts(void){
    int found = 0;

    printf("\n===== 我发布的帖子 =====\n");
    for(int i = 0; i < postCount; i++){
        if(strcmp(posts[i].publisherId, currentUser.ID) == 0){
            displayPost(&posts[i]);
            found = 1;
        }
    }

    if(!found){
        printf("You have not published any posts.\n");
    }

    pause();
}

