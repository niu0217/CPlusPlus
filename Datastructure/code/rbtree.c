#include<stdio.h>
#include<stdlib.h>

#define RED 1
#define BLACK 2
typedef int KEY_TYPE;

//定义红黑树节点的结构体
typedef struct _rbtree_node {
    unsigned char color;  //红色是1；黑色是2
    struct _rbtree_node* left;
    struct _rbtree_node* right;
    struct _rbtree_node* parent;
    KEY_TYPE key;
    void* value;  //用不到
}rbtree_node;

//定义红黑树
typedef struct _rbtree {
    rbtree_node* root;  //根指针
    rbtree_node* nil;  //外部指针
}rbtree;

//生成一个新的节点，并赋初值
rbtree_node* new_node(rbtree* T, int key) {
    rbtree_node* node = (rbtree_node*)malloc(sizeof(rbtree_node));
    node->color = RED; //新节点的最初颜色都是红色
    node->key = key;
    node->value = NULL;
    node->left = T->nil;
    node->right = T->nil;
    node->parent = T->nil;
    return node;
}

//左旋：
//  1.当前结点的右子树会作为新树的根结点
//  2.当前结点会作为新树的根结点的左子树
//  3.如果，新的树根，原来有左子树，原来的左子树，就作为旧根结点的右子树
void left_rotate(rbtree* T, rbtree_node* root) {
    rbtree_node* new_root = root->right;  //对应步骤1
    rbtree_node* root_rl = root->right->left;  //保存当前结点的右子树的左子树结点
    root->right = root_rl;  //对应步骤3

    //修改父结点
    //一共三个结点的父结点改变了：root_rl、new_root、root
    //修改root_rl的父结点
    if(root_rl != T->nil) {
        root_rl->parent = root;
    }
    //修改new_root的父结点
    new_root->parent = root->parent;
    if(root->parent == T->nil) {
        //说明，原来的root，就是整个树的根
        T->root = new_root;
    }
    else if(root == root->parent->left) {
        root->parent->left = new_root;
    }
    else if(root == root->parent->right) {
        root->parent->right = new_root;
    }
    //修改root的父结点
    new_root->left = root;  //对应步骤2
    root->parent = new_root;
}

//右旋：
//  1.当前结点的左子树会作为新树的根结点
//  2.当前结点会作为新树的根结点的右子树
//  3.如果，新的树根，原来有右子树，原来的右子树，就作为就根结点的左子树
void right_rotate(rbtree* T, rbtree_node* root) {
    rbtree_node* new_root = root->left;  //对应步骤1
    rbtree_node* root_lr = root->left->right;  //保存当前结点的左子树的右子树结点
    root->left = root_lr;  //对应步骤3

    //修改父结点
    //一共三个结点的父结点改变了：root_lr、new_root、root
    //修改root_lr的父结点
    if(root_lr != T->nil) {
        root_lr->parent = root;
    }
    //修改new_root的父结点
    new_root->parent = root->parent;
    if(root->parent == T->nil) {
        //说明，原来的root，就是整个树的根
        T->root = new_root;
    }
    else if(root == root->parent->left) {
        root->parent->left = new_root;
    }
    else if(root == root->parent->right) {
        root->parent->right = new_root;
    }
    //修改root的父结点
    new_root->right = root;  //对应步骤2
    root->parent = new_root;

}

//调整插入结点后的红黑树，避免失衡
void rbtree_insert_fixup(rbtree* T, rbtree_node* u) {
    while(u->parent->color == RED) {
        if(u->parent == u->parent->parent->left) {
            //LXx
            //获取插入结点的叔叔结点
            rbtree_node* y = u->parent->parent->right;
            if(y->color == RED) {
                //LXr
                //1.u的父亲结点变为黑色
                //2.u的叔叔结点变为黑色
                //3.u的祖父结点变为红色
                //4.将u更新为u的祖父结点，继续向上检查
                u->parent->color = BLACK;
                y->color = BLACK;
                u->parent->parent->color = BLACK;
                u = u->parent->parent;
            }
            else if(y->color == BLACK) {
                if(u == u->parent->right) {
                    //LRb
                    //1.先转换为LLb型
                    //  1.pu更新为u
                    //  2.对u子树进行左旋
                    //2.u的父亲结点变为黑色
                    //3.u的祖父结点变为红色
                    //4.对u的祖父结点为根的子树进行右旋
                    u = u->parent;
                    left_rotate(T, u);
                }
                //LLb
                //1.u的父亲结点变为黑色
                //2.u的祖父结点变为红色
                //3.对u的祖父结点为根的子树进行右旋
                u->parent->color = BLACK;
                u->parent->parent->color = RED;
                right_rotate(T, u->parent->parent);
            }
        }
        else if(u->parent == u->parent->parent->right) {
            //RXx
            //获取插入结点的叔叔结点
            rbtree_node* y = u->parent->parent->left;
            if(y->color == RED) {
                //RXr
                //1.u的父亲结点变为黑色
                //2.u的叔叔结点变为黑色
                //3.u的祖父结点变为红色
                //4.将u更新为u的祖父结点，继续向上检查
                u->parent->color = BLACK;
                y->color = BLACK;
                u->parent->parent->color = RED;
                u = u->parent->parent;
            }
            else if(y->color == BLACK) {
                if(u == u->parent->left) {
                    //RLb
                    //1.先转换为RRb型
                    //  1.pu更新为u
                    //  2.对u子树进行右旋
                    //2.u的父亲结点变为黑色
                    //3.u的祖父结点变为红色
                    //4.对u的祖父结点为根的子树进行右旋
                    u = u->parent;
                    right_rotate(T, u);
                }
                //RRb
                //1.u的父亲结点变为黑色
                //2.u的祖父结点变为红色
                //3.对u的祖父结点为根的子树进行左旋
                u->parent->color = BLACK;
                u->parent->parent->color = RED;
                left_rotate(T, u->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

//插入新结点
void rbtree_insert(rbtree* T, rbtree_node* z) {
    rbtree_node* y = T->nil;
    rbtree_node* x = T->root;
    while(x != T->nil) {
        y = x;  //y保存的是插入结点的父结点
        if(z->key < x->key) {
            x = x->left;  //在左子树中查找
        }
        else if(z->key > x->key) {
            x = x->right;  //在右子树中查找
        }
        else {
            return;  //不允许插入两个值相同的结点
        }
    }

    //找到z的父结点
    z->parent = y;
    //找到z应该插入的位置
    if(y == T->nil) {
        //如果要插入这个位置的父结点是空，则说明是根结点
        T->root = z;
    }
    else if(z->key < y->key) {
        y->left = z;
    }
    else if(z->key > y->key) {
        y->right = z;
    }

    //调整红黑树
    rbtree_insert_fixup(T, z);
}

//中序遍历
void mid_traversal(rbtree* T, rbtree_node* node) {
    if(node == T->nil) {
        return;
    }
    mid_traversal(T, node->left);
    printf("key:%d, color:%d\n", node->key, node->color);
    mid_traversal(T, node->right);
}

//先序遍历
void pre_traversal(rbtree* T, rbtree_node* node) {
    if(node == T->nil) {
        return;
    }
    printf("key:%d, color:%d\n", node->key, node->color);
    pre_traversal(T, node->left);
    pre_traversal(T, node->right);
}

int main()
{
    //10,50,60,62,65,70
    int key_array[6] = {10, 50, 60, 62, 65, 70};
    //创建一个红黑树
    rbtree* T = (rbtree*)malloc(sizeof(rbtree));
    //定义外部结点（NULL）
    T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
    T->nil->color = BLACK;
    T->root = T->nil;
    //红黑树插入结点
    rbtree_node* node = T->nil;
    for(int i = 0; i < 6; i++) {
        node = new_node(T, key_array[i]);
        rbtree_insert(T, node);
    }
    //打印插入的结点的结果
    printf("-----------------------中序遍历---------------------\n");
    mid_traversal(T, T->root);
    printf("-----------------------先序遍历---------------------\n");
    pre_traversal(T, T->root);
}
