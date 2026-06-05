#include "post.h"

char* getTypeName(int type){ 
    switch(type){ 
        case 1:return "Carpool"; 
        case 2:return "Group Buy"; 
        case 3:return "Study"; 
        case 4:return "Sports"; 
        case 5:return "Meal"; 
        case 6:return "Travel"; 
        case 7:return "Other"; 
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
        printf("ID:%d\n", posts[i].postId);
        printf("title:%s\n", posts[i].title);
        printf("Type:%s\n", getTypeName(posts[i].type));
        printf("Member: %d/%d\n", posts[i].current_number, posts[i].max_number);
        printf("Location:%s\n", posts[i].location);
        printf("Contact:%s\n", posts[i].contact);
        printf("Budget:%.2f\n", posts[i].budget);
        printf("Remark:%s\n", posts[i].remark);
    }

     fclose(fp);
}

void postAdd(void){
    post newPost;
    newPost.publishtime = time(NULL);//记录当前时间为发布时间
    newPost.current_number =1;//初始当前人数为1

    strcpy(newPost.publisherId, currentUser.ID);//把当前用户ID作为发布者ID
    
    newPost.postId = nextPostId++;//把当前编号给新帖子
    
    printf("Input title: ");
    scanf("%s", newPost.title);

    printf("Input type(1-7): ");
    scanf("%d", &newPost.type);

    printf("Input max number: ");
    scanf("%d", &newPost.max_number);

    printf("Gender limit (M/F/A): ");
    scanf(" %c", &newPost.genderlimit);

    printf("Input duration(days):");
    int days;
    scanf("%d", &days);

    newPost.start_time = time(NULL);
    newPost.end_time = newPost.start_time + days * 24 * 3600;//截止时间为开始时间加上持续天数

    if(newPost.current_number >= newPost.max_number){
        newPost.status = STATUS_FULL;
    }else{
        newPost.status = STATUS_ACTIVE;
    }

    printf("Input location:");
    scanf("%s", newPost.location);

    printf("Input contact:");
    scanf("%s", newPost.contact);

    printf("Input budget:");
    scanf("%lf", &newPost.budget);

    printf("Input remark:");
    scanf("%s", newPost.remark);

    printf("Post ID:%d\n", newPost.postId);
    printf("Title: %s\n", newPost.title);
    printf("Type: %s\n", getTypeName(newPost.type));
    printf("Member: %d/%d\n", newPost.current_number, newPost.max_number);
    printf("Location: %s\n", newPost.location);
    printf("Contact: %s\n", newPost.contact);
    printf("Budget: %.2f\n", newPost.budget);
    printf("Remark: %s\n", newPost.remark);
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
    printf("Post ID:%d\n", p->postId);
    printf("Publisher ID:%s\n", p->publisherId);
    printf("Title:%s\n", p->title);
    printf("Type:%s\n", getTypeName(p->type));
    printf("Current Number:%d/%d\n", p->current_number, p->max_number);
    printf("Gender Limit:%c\n", p->genderlimit);
    printf("Status:%s\n", getStatusName(p->status));
    printf("Start Time:%s\n", startStr);
    printf("End Time:%s\n", endStr);
    printf("Location:%s\n", p->location);
    printf("Contact:%s\n", p->contact);
    printf("Budget:%.2f\n", p->budget);
    printf("Remark:%s\n", p->remark);
    printf("Publish Time:%s\n", timeStr);
    printf("------------------------------\n");
    
}

void postListAll(void){
    sortPosts();
    for(int i = 0;i < postCount;i++){
        displayPost(&posts[i]);
    }
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

    printf("\n===== My Published Posts =====\n");
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

