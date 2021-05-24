#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct _TreeNode {
    struct _TreeNode *left;      // 比父节点小
    struct _TreeNode *right;     // 比父节点大

    int val;
} TreeNode;

static void __bst_show(TreeNode* parent, TreeNode* node, char *prefix) 
{
    sprintf(prefix, "%s|", prefix);
    if (node){
        printf("%s__%d" ,prefix, node->val);
        if (parent) {
            if (node == parent->left)  
                printf(" L\n");
            else 
                printf(" R\n");
        }
        else 
            printf("\n");

        if (node == parent || !parent || (parent && (node == parent->right || !parent->right))) 
            prefix[strlen(prefix)-1] = ' ';

        char *p = (char*)malloc(100);
        sprintf(p, "%s  ", prefix);
        __bst_show(node, node->left, p);
        sprintf(p, "%s  ", prefix);
        __bst_show(node, node->right, p);
        free(p);
    }
    // 用 # 代替空子节点
    // else {
    //     if (parent && (parent->left || parent->right))
    //         printf("%s__#\n", prefix);
    // }

}

void bst_show(TreeNode *root)
{
    if (root == NULL) return;
    printf("\n");

    char *p = (char *)malloc(100);
    memset(p, 0, 100);
    __bst_show(NULL, root, p);
    free(p);
}

void bst_init(TreeNode *c, int val)
{
    c->left = NULL;
    c->right = NULL;
    c->val = val;
}

TreeNode *bst_find(TreeNode *root, int val)
{
    TreeNode *node = root;

    // 访问到空节点就退出
    while (node) {
        if (val > node->val) {
            // 当前节点的值比参数的小，索引到右子节点
            node = node->right;
        } else if (val < node->val) {
            // 当前节点的值比参数的大，索引到左子节点
            node = node->left;
        } else { // 找到了值相等的节点 
            break;
        }
    }
    
    return node;
}

TreeNode *bst_insert(TreeNode *root, int val)
{
    // 如果是空树，申请一个节点作为根节点返回
    if (root == NULL) {
        root = (TreeNode *)malloc(sizeof(TreeNode));
        bst_init(root, val);
        return root;
    }
    
    TreeNode *node = root;
    while (1) {
        // 如果值比当前节点大
        if (val >= node->val) {
            // 如果其右子节点是否为空，就插入到这
            if (node->right == NULL) {
                node->right = (TreeNode *)malloc(sizeof(TreeNode));
                bst_init(node->right, val);
                break;
            }
            // 非空，继续遍历
            node = node->right;
        }
        else { // 如果值比当前节点小
            // 如果其左子节点是否为空，就插入到这
            if (node->left == NULL) {
                node->left = (TreeNode *)malloc(sizeof(TreeNode));
                bst_init(node->left, val);
                break; 
            }
            // 非空，继续遍历
            node = node->left;
        }
    }

    return root;
}

TreeNode *bst_delete(TreeNode *root, int val)
{
    TreeNode *node = root; 
    TreeNode *parent = NULL;

    // 访问到空节点就退出
    while (node) {
        if (val > node->val) {
            // 当前节点的值比参数的小，索引到右子节点
            parent = node;
            node = node->right;
        } else if (val < node->val) {
            // 当前节点的值比参数的大，索引到左子节点
            parent = node;
            node = node->left;
        } else { // 找到了值相等的节点 
            
            TreeNode *temp = NULL;
            // case 1 如果待删除节点拥有左右子节点，找到其右子树的最左节点
            if (node->left && node->right) {
                // 用于保存最左子节点的父节点
                TreeNode *temp_p = node->right;
                temp = node->right;
                // 寻找最左子节点，并记录其父节点
                while (temp->left) {
                    temp_p = temp;
                    temp = temp->left;
                }
                // 交换
                node->val = temp->val;
                // 如果待删除节点的右子节点是最左子节点，需要修改链接
                if (node->right == temp)
                    node->right = temp->right;
                // 判断最左子节点是否有有节子点，需要合并到父节点上
                if (temp->right) 
                    temp_p->left = temp->right;
                else 
                    temp_p->left = NULL;
                // 删除最左子节点
                free(temp);
                return root;
            } 

            // case 2 如果待删除的节点没有子节点，只需要删除此节点 
            if (!node->left && !node->right)
                temp = NULL;
            // case 3 如果待删除的节点只有左子节点，将其子节点链接到其父节点上
            else if (node->left && !node->right)             
                temp = node->left;
            // case 4 如果待删除的节点只有右子节点，将其子节点链接到其父节点上
            else if (!node->left && node->right) 
                temp = node->right;
            
            // 如果待删除节点是根节点
            if (node == root) 
                return temp;

            // 确定待删除节点是左还是右子节点
            if (node == parent->left) 
                parent->left = temp;
            else 
                parent->right = temp;

            // 删除节点
            free(node);
            
            return root;
        }
    }

    // 没有找到节点
    return root;
}


int main()
{
    int num;
    TreeNode *n = NULL;
    int mode=1;
    char str[20] = "\ninput: ";

    n = bst_insert(n, 5); bst_show(n);
    n = bst_insert(n, 3); bst_show(n);
    n = bst_insert(n, 6); bst_show(n);
    n = bst_insert(n, 2); bst_show(n);
    n = bst_insert(n, 4); bst_show(n);
    n = bst_insert(n, 7); bst_show(n);
    n = bst_delete(n, 3); bst_show(n);
    
//     while (1)
//     {
//         printf("%s",str);
//         scanf("%d", &num);
//         switch (num) {
//             case -1 :
//                 mode = 1;
//                 sprintf(str, "\ninput: ");
//                 break;
//             case -2 :
//                 mode = 0;
//                 sprintf(str, "\ndelete: ");
//                 break;
//             case -3 :
//                 goto exit;
//             default :
//                 if (mode)
//                     bst_insert(&n, num);
//                 else 
//                     bst_delete(&n, num);
//                 break;
//         }        

//         bst_show(n);
//     }
// exit: 
//     printf("exit\n");    
}
