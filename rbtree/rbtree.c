#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

/*
 * 性质1 结点是红色或黑色。
 * 性质2 根结点是黑色。
 * 性质3 所有叶子都是黑色。（叶子是rbt->nil结点）
 * 性质4 如果一个节点是红色的，则它的子节点必须是黑色的。
 * 性质5 每个节点到该节点的所有后代节点的简单路径上,包含相同数目的黑节点。
 */ 

typedef struct _TreeNode {
    struct _TreeNode *parent;
    struct _TreeNode *left;     // 比父节点小
    struct _TreeNode *right;    // 比父节点大
    int color;                  // 0 黑 1 红
    int val;
} TreeNode;

typedef struct {
    TreeNode *root;
    TreeNode *nil;
} RBTree;


#define RBT_RED     1
#define RBT_BLACK   0

static void __rbt_show(RBTree* rbt, TreeNode* parent, TreeNode* node, char *prefix) 
{
    sprintf(prefix, "%s\\", prefix);
    if (node != rbt->nil) {
        printf("\033[40;32;5m%s___\033[0m" ,prefix);
        if (node->color)
            printf("\033[40;35;5m%d", node->val);
        else
            printf("\033[40;34;5m%d", node->val);
        prefix[strlen(prefix)-1] = '|';
        if (parent != rbt->nil) {
            if (node == parent->left)  
                printf(" L\033[0m\n");
            else 
                printf(" R\033[0m\n");
        }
        else 
            printf("\033[0m\n");

        if (node == parent || parent == rbt->nil || node == parent->left)
            prefix[strlen(prefix)-1] = ' ';

        char *p = (char*)malloc(100);
        sprintf(p, "%s   ", prefix);
        __rbt_show(rbt, node, node->right, p);
        sprintf(p, "%s   ", prefix);
        __rbt_show(rbt, node, node->left, p);
        free(p);
    }
    // 用 # 代替空子节点
    else {
        if (parent->left != rbt->nil || parent->right != rbt->nil)
            printf("\033[40;32;5m%s___\033[0m\033[40;34;5m#\033[0m\n", prefix);
    }

}

void rbt_show(RBTree *rbt)
{
    printf("\n\033[42;32;5m \033[0m\n");
    if (rbt->root == rbt->nil) {
        printf("\033[40;32;5m\\__empty\033[0m\n");
        return;
    }
    char *p = (char *)malloc(100);
    memset(p, 0, 100);
    __rbt_show(rbt, rbt->nil, rbt->root, p);
    free(p);
}

static inline void rbt_init(RBTree *rbt, TreeNode *n, TreeNode *parent, int val)
{
    n->parent = parent;
    n->left  = rbt->nil;
    n->right = rbt->nil;
    n->val   = val;
    n->color = RBT_RED;
}

TreeNode * rbt_find(RBTree *rbt, int val)
{
    TreeNode *node = rbt->root;

    // 访问到空节点就退出
    while (node) {
        if (val > node->val) 
            // 当前节点的值比参数的小，索引到右子节点
            node = node->right;
        else if (val < node->val) 
            // 当前节点的值比参数的大，索引到左子节点
            node = node->left;
        else  // 找到了值相等的节点 
            break;
    }
    
    return node;
}

static void rbt_rotate_left(RBTree *rbt, TreeNode *node)
{
    if (!node->right) return;
    
    TreeNode *rnode = node->right;
    // 修改节点，进行旋转
    node->right   = rnode->left;
    rnode->left   = node;
    rnode->parent = node->parent;
    node->parent  = rnode;

    // 判断是否为根节点
    if (rnode->parent == rbt->nil) {
        rbt->root = rnode;
        return;
    }
    // 修改父节点信息
    if (node == rnode->parent->left)
        rnode->parent->left  = rnode;
    else 
        rnode->parent->right = rnode;
}

static void rbt_rotate_right(RBTree *rbt, TreeNode *node)
{
    if (!node->left) return;
    
    TreeNode *lnode = node->left;
    // 修改节点信息，进行旋转
    node->left    = lnode->right;
    lnode->right  = node;
    lnode->parent = node->parent;
    node->parent  = lnode;

    // 判断是否为根节点
    if (lnode->parent == rbt->nil) {
        rbt->root = lnode;
        return;
    }
    // 修改父节点信息
    if (node == lnode->parent->left)
        lnode->parent->left = lnode;
    else 
        lnode->parent->right = lnode;
}


static void rbt_insert_fixup(RBTree *rbt, TreeNode *node)
{
    // 叔父节点
    TreeNode *uncle, *parent, *gparent;
    // 根节点不需要修复
    if (node == rbt->root)
        return;
    // 一层也不需要修复
    if (node->parent == rbt->root)
        return;
    // 直到指针节点的父节点是黑色
    while (node->parent->color == RBT_RED) {
        // 记录父节点
        parent  = node->parent;
        // 记录祖父节点
        gparent = parent->parent;
        // 获得叔父节点
        uncle = (parent==gparent->left)?gparent->right:gparent->left;

        // case 1 父节点和叔父节点为红色，将他们重新着色为黑色
        if (uncle && uncle->color == RBT_RED) {
            parent->color  = RBT_BLACK;
            uncle->color   = RBT_BLACK;
            gparent->color = RBT_RED;
            // 节点指针上升到祖父节点
            node = gparent;
            continue;
        }

        // 父节点是红的而叔父节点是黑的，需要进行旋转
        if (parent == gparent->left) {
            // case 2 父节点是左子节点且当前节点为右子节点时，进行左旋转
            if (node == parent->right) {
                node = parent;
                rbt_rotate_left(rbt, node);
                // 更新父节点，祖父节点不变
                parent = node->parent;
            }
            // case 3 父节点是左子节点且当前节点为左子节点时，进行右旋转
            parent->color  = RBT_BLACK;
            gparent->color = RBT_RED;
            rbt_rotate_right(rbt, gparent);

        } else {
            // case 2 父节点是右子节点，当前节点为左子节点时，进行右旋转
            if (node == node->parent->left) {
                node = node->parent;
                rbt_rotate_right(rbt, node);
                // 更新父节点，祖父节点不变
                parent = node->parent;
            }
            // case 3 父节点是右子节点且当前节点为左子节点时，进行左旋转
            parent->color  = RBT_BLACK;
            gparent->color = RBT_RED;
            rbt_rotate_left(rbt, gparent);
        }
    }
    // 保持根节点是黑色
    rbt->root->color = RBT_BLACK;
    
    return;
}

void rbt_insert(RBTree *rbt, int val)
{
    // 如果是空树，申请一个节点作为根节点返回
    if (rbt->root == rbt->nil) {
        // 哨兵
        rbt->nil = (TreeNode *)malloc(sizeof(TreeNode));
        rbt_init(rbt, rbt->nil, rbt->nil, -1);
        rbt->nil->color = RBT_BLACK;
        // 根
        rbt->root = (TreeNode *)malloc(sizeof(TreeNode));
        rbt_init(rbt, rbt->root, rbt->nil, val);
        rbt->root->color = RBT_BLACK;
        return;
    }
    
    TreeNode *node = rbt->root;
    while (1) {
        // 如果值比当前节点大
        if (val >= node->val) {
            // 如果其右子节点是否为空，就插入到这
            if (node->right == rbt->nil) {
                node->right = (TreeNode *)malloc(sizeof(TreeNode));
                rbt_init(rbt, node->right, node, val);
                rbt_insert_fixup(rbt, node->right); 
                break;  
            }
            // 非空，继续遍历
            node = node->right;
        }
        else { // 如果值比当前节点小
            // 如果其左子节点是否为空，就插入到这
            if (node->left == rbt->nil) {
                node->left = (TreeNode *)malloc(sizeof(TreeNode));
                rbt_init(rbt, node->left, node, val);
                rbt_insert_fixup(rbt, node->left); 
                break; 
            }
            // 非空，继续遍历
            node = node->left;
        }
    }
    return;
}

void rbt_delete_fixup(RBTree *rbt, TreeNode *node)
{
    TreeNode *parent, *brother;

    // 修复
    while (node != rbt->root && node->color == RBT_BLACK) {
        parent = node->parent;
        // 当前节点为左子节点
        if (node == parent->left) {
            // 获取兄弟节点
            brother = parent->right;
            if (brother == rbt->nil) continue;
            // case 1 兄弟节点是红的，对父节点进行左旋
            if (brother && brother->color == RBT_RED) {
                brother->color = RBT_BLACK;
                parent->color  = RBT_RED;
                rbt_rotate_left(rbt, parent);
                brother = parent->right;
            }
            // case 2 兄弟节点为黑色，左右子节点都是黑的，当前节点变红
            else if (brother->left->color == RBT_BLACK && brother->right->color == RBT_BLACK) {
                brother->color = RBT_RED;
                node = parent;
            }
            // case 3 兄弟节点为黑色，右子节点是黑色，
            else if (brother->right->color == RBT_BLACK) {
                brother->color = RBT_RED;
                brother->left->color = RBT_BLACK;
                rbt_rotate_right(rbt, brother);
                brother = node->parent->right;        
            }
            // case 4 兄弟节点为黑色，右子节点为红色
            else {
                brother->color = parent->color;
                parent->color  = RBT_BLACK;
                brother->right->color = RBT_BLACK;
                rbt_rotate_left(rbt, parent);
                node = rbt->root;
            }
        }
        // 当前节点为右子节点
        else {
            // 获取兄弟节点
            brother = parent->left;
            // case 1 兄弟节点是红的，对父节点进行左旋
            if (brother->color == RBT_RED) {
                node->color   = RBT_BLACK;
                parent->color = RBT_RED;
                rbt_rotate_left(rbt, parent);
                node = parent->left;
            }
            // case 2 兄弟节点为黑色，左右子节点都是黑的，当前节点变红
            else if (brother->right->color == RBT_BLACK && brother->left->color == RBT_BLACK) {
                brother->color = RBT_RED;
                node = parent;
            }
            // case 3 兄弟节点为黑色，右子节点是黑色，
            else if (brother->left == RBT_BLACK) {
                brother->color = RBT_RED;
                brother->right->color = RBT_BLACK;
                rbt_rotate_left(rbt, brother);
                brother = node->parent->left;        
            }
            // case 4 兄弟节点为黑色，左子节点为红色
            else {
                brother->color = parent->color;
                parent->color  = RBT_BLACK;
                node->left->color = RBT_BLACK;
                rbt_rotate_right(rbt, parent);
                node = rbt->root;
            }
        }
    }
    node->color = RBT_BLACK;
}

void rbt_delete(RBTree *rbt, int val)
{
    TreeNode *node = rbt->root; 
    TreeNode *parent = rbt->nil;

    // 访问到空节点就退出
    while (node) {
        if (val > node->val) {
            // 当前节点的值比参数的小，索引到右子节点
            parent = node;
            node   = node->right;
        } else if (val < node->val) {
            // 当前节点的值比参数的大，索引到左子节点
            parent = node;
            node   = node->left;
        } else { // 找到了值相等的节点 
            TreeNode *fix = NULL;
            TreeNode *replace = NULL;
            int color = node->color;

            // case 1 如果当前节点拥有左右子节点，找到其右子树的最左节点，替换当前的节点
            if (node->left != rbt->nil && node->right != rbt->nil) {
                // 记录最左子节点，用于与当前节点交换
                replace = node->right;
                // 寻找最左子节点
                while (replace->left != rbt->nil) replace = replace->left;
                // 记录待修正的节点和待删除节点的颜色
                fix   = replace->right;
                color = replace->color;
                // 判断最左子节点是否为当前节点的右子节点
                if (replace->parent == node) {
                    // 有可能是 nil
                    fix->parent = replace;
                } else {
                    // 将最左子节点的右子节点接到其父节点的左子节点处
                    replace->parent->left = fix;
                    fix->parent = replace->parent;
                    node->right->parent = replace;
                    replace->right = node->right;
                }
                // 交换最左子节点和当前节点
                replace->parent = node->parent;
                replace->color  = node->color;
                replace->left   = node->left;
                replace->left->parent = replace;

            } else {
                // case 2 如果当前的节点只有左子节点，将其子节点链接到其父节点上
                if (node->left != rbt->nil && node->right == rbt->nil)             
                    replace = node->left;
                // case 3 如果当前的节点只有右子节点，将其子节点链接到其父节点上
                else if (node->left == rbt->nil && node->right != rbt->nil) 
                    replace = node->right;
                // case 4 如果当前的节点没有子节点，只需要删除此节点 
                else {
                    replace = rbt->nil;
                }
                // 记录待修正的节点和待删除节点的颜色
                fix   = replace;
                color = node->color;
            }
            
            // 如果当前节点是根节点
            if (node == rbt->root) {
                rbt->root = replace;
                replace->parent = rbt->nil;
            } else {
                // 更新父节点
                replace->parent = parent;
                // 确定当前节点是左还是右子节点
                if (node == parent->left) 
                    parent->left = replace;
                else 
                    parent->right = replace;
            }

            // 黑色节点需要修复，红色不需要
            // printf("\nbefore fix"); rbt_show(rbt);
            if (color == RBT_BLACK) {
                rbt_delete_fixup(rbt, fix);
            }
            // printf("\nafter fix");

            // 删除节点
            free(node);           
            return;
        }
    }

    // 没有找到节点
}

int main()
{
    int num;
    RBTree n = {NULL};
    int mode=1;
    char str[20] = "\ninput: ";
    int in[]  = {41,38,31,12,19,8};
    int del[] = {8,12,19,31,38,41};

    for (int i=0; i<sizeof(in)/sizeof(in[0]); i++) {
        printf("\n---%d insert: %d", i, in[i]);
        rbt_insert(&n, in[i]);
        rbt_show(&n);
    }    
    printf("\n_________________________________");
    printf("\n| | | | | | | | | | | | | | | | |\n");
    for (int i=0; i<sizeof(del)/sizeof(del[0]); i++) {
        printf("\n---%d delete: %d", i, del[i]);
        rbt_delete(&n, del[i]);
        rbt_show(&n);
    }

    // for (int i=0; i<20; i++) {
    //     num = rand()%49;
    //     printf("\n%d input: %d", i, num);
    //     rbt_insert(&n, num);
    //     // if (i>5 && i<21) 
    //         rbt_show(&n);
    // }
    //rbt_show(&n);
    
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
//                     rbt_insert(&n, num);
//                 else 
//                     rbt_delete(&n, num);
//                 break;
//         }        

//         rbt_show(n);
//     }
// exit: 
//     printf("exit\n");    
}
