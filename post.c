#include "post.h"

post posts[MAX_POSTS];
int postCount = 0;//帖子数组和计数器
int nextPostId = 1;//下一个帖子ID

char* getTypeName(int type){
    switch(type){
        case 1:return "Carpool";
        case 2:return "Group Buy";
        case 3:return "Study";
        case 4:return "Sports";
        case 5:return "Meal";
        case 6:return "Travel";
        default:return "Other";
    }
}

void sortPosts(void){
    for(int i = 0; i < postCount - 1; i++){
        for(int j = 0; j < postCount - i - 1; j++){
            if(posts[j].postId < posts[j + 1].postId){
                post temp = posts[j];
                posts[j] = posts[j + 1];
                posts[j + 1] = temp;
            }
        }
    }
}

void loadPosts(void){
    postCount = 0;

    FILE *fp = fopen("data/posts.txt","r");
    
    if(fp == NULL){
        printf("File Open Failed!\n");
        return;
    }

    printf("File Open Success!\n");
    
    while(fscanf(fp,"%d|%[^|]|%d|%d|%[^|]|%[^|]|%lf|%[^\n]\n", //%[^|\n]表示读取直到遇到|或换行的字符串停止
           &posts[postCount].postId,
           posts[postCount].title,
           &posts[postCount].current_number,
           &posts[postCount].max_number,
           posts[postCount].location,
           posts[postCount].contact,
           &posts[postCount].budget,
           posts[postCount].remark) == 8)
    {
        printf("Load One Post\n");
        postCount++;
        if(posts[postCount - 1].postId >= nextPostId){
            nextPostId = posts[postCount - 1].postId + 1;
        }
    }
    
    for(int i = 0; i < postCount; i++){
        printf("ID:%d\n", posts[i].postId);
        printf("title:%s\n", posts[i].title);
        printf("Current Number:%d\n", posts[i].current_number);
        printf("Max Number:%d\n", posts[i].max_number);
        printf("Location:%s\n", posts[i].location);
        printf("Contact:%s\n", posts[i].contact);
        printf("Budget:%.2f\n", posts[i].budget);
        printf("Remark:%s\n", posts[i].remark);
    }

     fclose(fp);
}

void postAdd(void){
    post newPost;
    newPost.current_number =1;//初始当前人数为1
    
    if(newPost.current_number >= newPost.max_number){
        newPost.status = STATUS_FULL;
    }else{
        newPost.status = STATUS_ACTIVE;
    }
    
    newPost.postId = nextPostId;//把当前编号给新帖子
    nextPostId++;

    printf("Input title: ");
    scanf("%s", newPost.title);

    printf("Input type(1-7): ");
    scanf("%d", &newPost.type);

    printf("Input max number: ");
    scanf("%d", &newPost.max_number);

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
    printf("Current Number: %d\n", newPost.current_number);
    printf("Max Number: %d\n", newPost.max_number);
    printf("Location: %s\n", newPost.location);
    printf("Contact: %s\n", newPost.contact);
    printf("Budget: %.2f\n", newPost.budget);
    printf("Remark: %s\n", newPost.remark);
    posts[postCount] = newPost;//把新帖子放到数组里
    postCount++;

    savePosts();//保存到文件

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
        fprintf(fp,"%d|%s|%d|%d|%s|%s|%.2lf|%s\n",
            posts[i].postId,
            posts[i].title,
            posts[i].current_number,
            posts[i].max_number,
            posts[i].location,
            posts[i].contact,
            posts[i].budget,
            posts[i].remark);
    }
    
    fclose(fp);
}

void displayPost(post *p){
        printf("\n");
        printf("Post ID:%d\n", p->postId);
        printf("Title:%s\n", p->title);
        printf("Type:%s\n", getTypeName(p->type));
        printf("Current Number:%d/%d\n", p->current_number, p->max_number);
        printf("Location:%s\n", p->location);
        printf("Contact:%s\n", p->contact);
        printf("Budget:%.2f\n", p->budget);
        printf("Remark:%s\n", p->remark);
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
    return NULL;
}

void updatePost(post *p){
    savePosts();
    
    p -> current_number++;
    if(p -> current_number >= p -> max_number){
        p -> status = STATUS_FULL;
    }
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
