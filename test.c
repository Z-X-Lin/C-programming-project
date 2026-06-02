#include "post.h"

int main(void)
{
    loadPosts();

    post *p = getPostById(1);//根据ID获取帖子，假设ID为1

    if(p == NULL)
    {
        printf("Post Not Found!\n");
        return 0;
    }

    printf("===== Before =====\n");
    displayPost(p);

    updatePost(p);

    printf("\n===== After =====\n");
    displayPost(p);

    return 0;
}