#include <stdio.h>
#include <stdlib.h>

typedef enum {
    NODE_CONST, // константа
    NODE_VAR,   // переменная
    NODE_OP,     // операция
} NodeType;

typedef enum {
    OP_ADD,
    OP_MUL,
} Operation;

struct Node {
    NodeType type;
    union {
        int value;
        char varName;
        struct {
            Operation op;
            struct Node* left;
            struct Node* right;
        } binop;
    } data;
};

struct Node* createConst(int val) {
    struct Node* node = malloc(sizeof(struct Node));
    node->type = NODE_CONST;
    node->data.value = val;
    return node;
}

struct Node* createVar(char name) {
    struct Node* node = malloc(sizeof(struct Node));
    node->type = NODE_VAR;
    node->data.varName = name;
    return node;
}

struct Node* createOp(Operation op, struct Node* left, struct Node* right) {
    struct Node* node = malloc(sizeof(struct Node));
    node->type = NODE_OP;
    node->data.binop.op = op;
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

void deleteTree(struct Node* root) {
    if (!root) return;
    if (root->type == NODE_OP) {
        deleteTree(root->data.binop.left);
        deleteTree(root->data.binop.right);
    }
    free(root);
}

struct Node* expandMulToSum(struct Node* root) { // предполагаем выражение n * a
    if (!root || root->type != NODE_OP || root->data.binop.op != OP_MUL)
        return root;

    struct Node* left = root->data.binop.left;   // переменная a
    struct Node* right = root->data.binop.right; // константа n

    if (!right || right->type != NODE_CONST || !left || left->type != NODE_VAR)
        return root;

    int n = right->data.value;

    if (n <= 0) {
        deleteTree(left);
        deleteTree(right);
        free(root);
        return createConst(0);
    }

    struct Node* sum = createVar(left->data.varName);
    for (int i = 1; i < n; i++) {
        struct Node* nextVar = createVar(left->data.varName);
        sum = createOp(OP_ADD, sum, nextVar);
    }

    deleteTree(left);
    deleteTree(right);
    free(root);
    return sum;
}

void printExpr(struct Node* root) {
    if (!root) return;

    if (root->type == NODE_CONST) {
        printf("%d", root->data.value);
    } else if (root->type == NODE_VAR) {
        printf("%c", root->data.varName);
    } else { // NODE_OP
        printExpr(root->data.binop.left);
        if (root->data.binop.op == OP_ADD)
            printf(" + ");
        else
            printf(" * ");
        printExpr(root->data.binop.right);
    }
}

struct Node* buildExample(void) {// исходное выражение: 3 * a
    struct Node* three = createConst(3);
    struct Node* a = createVar('a');
    return createOp(OP_MUL, three, a);
}

int main(void) {
    struct Node* expr = buildExample();
    printf("Исходное выражение: ");
    printExpr(expr);
    printf("\n");

    struct Node* reducted = expandMulToSum(expr);
    printf("После редaкции: ");
    printExpr(reducted);
    printf("\n");

    deleteTree(reducted);
    return 0;
}