#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode* createNode(int value) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int allLeavesInRange(struct TreeNode* root, int a, int b) {
    if (root == NULL) {
        return 1;
    }
    
    if (root->left == NULL && root->right == NULL) {
        return (root->val >= a && root->val <= b);
    }
    
    return allLeavesInRange(root->left, a, b) && allLeavesInRange(root->right, a, b);
}

struct TreeNode* insert(struct TreeNode* node, int value) {
    if (node == NULL) {
        return createNode(value);
    }

    if (value < node->val) {
        node->left = insert(node->left, value); 
    } else if (value >= node->val) {
        node->right = insert(node->right, value); 
    }

    return node;
}

void deleteTree(struct TreeNode* node) {
    if (node == NULL) {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);

    printf("Удаление узла с данными: %d\n", node->val);
    free(node);
}

int main() {
    struct TreeNode* node = NULL;
    char command;

    while (1) {
        printf("Введите команду: М для добавления, С для проверки, Q чтобы закончить\n");
        scanf(" %c", &command);
        command = toupper(command);

        if (command == 'M') {
            int value;
            printf("Введите число\n");
            scanf("%d", &value);
            node = insert(node, value);
        }

        else if (command == 'C') {
            int a, b;
            printf("Введите диапазон чисел\n");
            scanf("%d %d", &a, &b);

            if (allLeavesInRange(node, a, b)){
                printf("Все листья в диапазоне [%d, %d]\n", a, b);
            } else {
                printf("Не все листья в диапазоне [%d, %d]\n", a, b);
            }
        }
            
        else if (command == 'Q'){
            deleteTree(node);
            break;
            }
        else {
            printf("Неправильная команда\n");
        }
    }

    return 0;
}